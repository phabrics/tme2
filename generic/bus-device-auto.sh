#! /bin/sh

# $Id: bus-device-auto.sh,v 1.3 2009/08/29 17:52:04 fredette Exp $

# generic/bus-device-auto.sh - automatically generates C code for
# generic bus device support:

#
# Copyright (c) 2004 Matt Fredette
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#      This product includes software developed by Matt Fredette.
# 4. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

header=false

for option
do
    case $option in
    --header) header=true ;;
    esac
done

PROG=`basename $0`
cat <<EOF
/* automatically generated by $PROG, do not edit! */
_TME_RCSID("\$Id: bus-device-auto.sh,v 1.3 2009/08/29 17:52:04 fredette Exp $");
EOF

if $header; then :; else
    cat <<EOF

/* this gives the number of entries that must be in a generic bus
   router array for a device with a bus size of 8 * (2 ^ siz_lg2)
   bits: */
#define TME_BUS_ROUTER_INIT_SIZE(siz_lg2)                       \\
  TME_BUS_ROUTER_INIT_INDEX(siz_lg2, (1 << (siz_lg2)) + 1, 0)

EOF
fi

# permute over initiator bus port width:
#
i_width=8
while test ${i_width} != 32; do
    i_width=`expr ${i_width} \* 2`

    # permute over initiator endianness:
    #
    for endian in b l; do
	if test ${endian} = b; then endian_what=big; else endian_what=little; fi

	# start the array:
	#
	echo ""
	echo "/* the ${i_width}-bit ${endian_what}-endian bus master bus router: */"
	what="const tme_bus_lane_t tme_bus_device_router_${i_width}e${endian}"
	if $header; then
	    echo "extern ${what}[];"
	    continue
	fi
	echo "${what}[TME_BUS_ROUTER_INIT_SIZE(TME_BUS${i_width}_LOG2)] = {"

	# permute over initiator maximum cycle size:
	#
	i_size=0
	while test `expr ${i_size} \< ${i_width}` = 1; do
	    i_size=`expr ${i_size} + 8`

	    # permute over initiator address offset:
	    #
	    i_offset=0
	    while test `expr ${i_offset} \< ${i_width}` = 1; do

		# calculate the initiator least and greatest lanes:
		#
		placeholder=false
		if test ${endian} = b; then
		    i_lane_greatest=`expr 0 - 8 + ${i_width} - ${i_offset}`
		    i_lane_least=`expr 8 + \( ${i_lane_greatest} \) - ${i_size}`
		    if test `expr \( ${i_lane_least} \) \< 0` = 1; then
			placeholder=true
		    fi
		else
		    i_lane_least=$i_offset
		    i_lane_greatest=`expr 0 - 8 + ${i_offset} + ${i_size}`
		    if test `expr \( ${i_lane_greatest} \) \>= ${i_width}` = 1; then
			placeholder=true
		    fi
		fi

		# permute over responder bus port width:
		#
		r_width=4
		while test `expr ${r_width} \< ${i_width}` = 1; do
		    r_width=`expr ${r_width} \* 2`

		    # permute over responder bus port least lane:
		    #
		    r_lane_least=0
		    while test `expr ${r_lane_least} \< ${i_width}` = 1; do
			r_lane_greatest=`expr 0 - 8 + ${r_lane_least} + ${r_width}`

			# emit the initiator information:
			#
			echo ""
			echo "  /* initiator maximum cycle size: ${i_size} bits"
			echo "     initiator address offset: ${i_offset} bits"
			if $placeholder; then
			    echo "     (a ${i_width}-bit initiator cannot request ${i_size} bits at an ${i_offset}-bit offset - this is an array placeholder)"
			fi

			# emit the responder information:
			#
			echo "     responder bus port size: ${r_width} bits"
			echo -n "     responder port least lane: D"`expr ${r_lane_least} + 7`"-D${r_lane_least}"

			# if the responder bus port greatest lane is
			# greater than the initiator bus port width,
			# part of the responder's port is outside of
			# the initiator's port:
			#
			if test `expr \( ${r_lane_greatest} \) \>= ${i_width}` = 1; then
			    echo ""
			    echo -n "     (responder port not correctly positioned for this initiator)"
			fi
			echo ": */"

			# permute over the lanes:
			#
			lane=0
			if test ${endian} = b; then
			    route=`expr ${i_size} / 8`
			    route_increment=-1
			else
			    route=-1
			    route_increment=1
			fi
			while test `expr ${lane} \< ${i_width}` = 1; do
			    echo -n "  /* D"`expr ${lane} + 7`"-D${lane} */	"

			    # see if this lane is on in the responder:
			    #
			    if test `expr ${lane} \>= ${r_lane_least}` = 1 \
			       && test `expr ${lane} \<= \( ${r_lane_greatest} \)` = 1; then
				r_lane_on=true
			    else
				r_lane_on=false
			    fi

			    # see if this lane is on in the initiator:
			    #
			    if test `expr ${lane} \>= \( ${i_lane_least} \)` = 1 \
			       && test `expr ${lane} \<= \( ${i_lane_greatest} \)` = 1; then
			        i_lane_on=true
				route=`expr \( ${route} + ${route_increment} \)`
			    else
				i_lane_on=false
			    fi

			    # if this is a placeholder entry:
			    #
			    if $placeholder; then
				echo -n "TME_BUS_LANE_ABORT"

			    # otherwise, this is a real entry:
			    #
			    else
				if $i_lane_on; then
				    echo -n "TME_BUS_LANE_ROUTE(${route})"
				    if $r_lane_on; then :; else
					echo -n " | TME_BUS_LANE_WARN"
				    fi
				else
				    echo -n "TME_BUS_LANE_UNDEF"
				fi
			    fi

			    echo ","
			    lane=`expr ${lane} + 8`
			done

			r_lane_least=`expr ${r_lane_least} + 8`
		    done
		done
		
		i_offset=`expr ${i_offset} + 8`
	    done
	done

	# finish the array:
	#
	echo "};"
    done

    # permute over read/write:
    #
    for name in read write; do
	capname=`echo $name | tr a-z A-Z`
	if test $name = read; then 
	    naming="reading"
	    from="from"
	    constbuffer=""
	else
	    naming="writing"
	    from="to"
	    constbuffer="const "
	fi

	echo ""
	echo "/* the ${i_width}-bit bus master DMA ${name} function: */"
	if $header; then
	    echo "int tme_bus_device_dma_${name}_${i_width} _TME_P((struct tme_bus_device *,"
	    echo "                                       tme_bus_addr_t,"
	    echo "                                       tme_bus_addr_t,"
	    echo "                                       ${constbuffer}tme_uint8_t *,"
	    echo "                                       unsigned int));"
	    continue
	fi
	echo "int"
	echo "tme_bus_device_dma_${name}_${i_width}(struct tme_bus_device *bus_device,"
	echo "                           tme_bus_addr_t address_init,"
	echo "                           tme_bus_addr_t size,"
	echo "                           ${constbuffer}tme_uint8_t *buffer,"
	echo "                           unsigned int locks)"
	echo "{"
	echo "  struct tme_bus_tlb *tlb, tlb_local;"
	echo "  struct tme_bus_connection *conn_bus;"
	echo "  tme_bus_addr_t count_minus_one, count;"
	echo "  struct tme_bus_cycle cycle;"
	echo "  tme_bus_addr_t address_resp;"
	echo "  int shift;"
	echo "  int err;"
	echo ""
	echo "  /* assume no error: */"
	echo "  err = TME_OK;"
	echo ""
	echo "  /* loop while we have more bytes to ${name}: */"
	echo "  for (; err == TME_OK && size > 0; ) {"
	echo ""
	echo "    /* hash this address into a TLB entry: */"
	echo "    tlb = (*bus_device->tme_bus_device_tlb_hash)"
	echo "            (bus_device,"
	echo "             address_init,"
	echo "             TME_BUS_CYCLE_${capname});"
	echo ""
	echo "    /* busy this TLB entry: */"
	echo "    tme_bus_tlb_busy(tlb);"
	echo ""
	echo "    /* if this TLB entry is invalid, doesn't cover this address, or if it doesn't"
	echo "       allow ${naming}, reload it: */"
	echo "    if (tme_bus_tlb_is_invalid(tlb)"
	echo "        || address_init < tlb->tme_bus_tlb_addr_first"
	echo "        || address_init > tlb->tme_bus_tlb_addr_last"
	echo "        || (tlb->tme_bus_tlb_emulator_off_${name} == TME_EMULATOR_OFF_UNDEF"
	echo "            && !(tlb->tme_bus_tlb_cycles_ok & TME_BUS_CYCLE_${capname}))) {"
	echo ""
	echo "      /* unbusy this TLB entry for filling: */"
	echo "      tme_bus_tlb_unbusy_fill(tlb);"
	echo ""
	echo "      /* pass this TLB's token: */"
	echo "      tlb_local.tme_bus_tlb_token = tlb->tme_bus_tlb_token;"
	echo ""
	echo "      /* get our bus connection: */"
	echo "      conn_bus = tme_memory_atomic_pointer_read(struct tme_bus_connection *,"
	echo "                                                bus_device->tme_bus_device_connection,"
	echo "                                                &bus_device->tme_bus_device_connection_rwlock);"
	echo ""
	echo "      /* unlock the device: */"
	echo "      (*bus_device->tme_bus_device_unlock)(bus_device, locks);"
	echo ""
	echo "      /* reload the TLB entry: */"
	echo "      err = (*conn_bus->tme_bus_tlb_fill)"
	echo "              (conn_bus,"
	echo "               &tlb_local,"
	echo "               address_init,"
	echo "               TME_BUS_CYCLE_${capname});"
	echo ""
	echo "      /* lock the device: */"
	echo "      (*bus_device->tme_bus_device_lock)(bus_device, locks);"
	echo ""
	echo "      /* return if we couldn't fill the TLB entry: */"
	echo "      if (err != TME_OK) {"
	echo "        return (err);"
	echo "      }"
	echo ""
	echo "      /* store the TLB entry: */"
	echo "      *tlb = tlb_local;"
	echo ""
	echo "      /* loop to check the newly filled TLB entry: */"
	echo "      continue;"
	echo "    }"
	echo ""
	echo "    /* if this TLB entry allows fast ${naming}: */"
	echo "    if (tlb->tme_bus_tlb_emulator_off_${name} != TME_EMULATOR_OFF_UNDEF) {"
	echo ""
	echo "      /* see how many bytes we can fast ${name} ${from} this TLB entry,"
	echo "         starting at this address: */"
	echo "      count_minus_one = (tlb->tme_bus_tlb_addr_last - address_init);"
	echo ""
	echo "      /* ${name} that many bytes or size bytes, whichever is smaller: */"
	echo "      count_minus_one = TME_MIN(count_minus_one,"
	echo "                                (size - 1));"
	echo "      count = count_minus_one + 1;"
	echo "      assert (count != 0);"
	echo ""
	echo "      /* do the bus ${name}: */"
	echo "      tme_memory_bus_${name}_buffer((tlb->tme_bus_tlb_emulator_off_${name} + address_init), buffer, count, tlb->tme_bus_tlb_rwlock, sizeof(tme_uint8_t), sizeof(tme_uint${i_width}_t));"
	echo ""
	echo "      /* unbusy this TLB entry: */"
	echo "      tme_bus_tlb_unbusy(tlb);"
	echo "    }"
	echo ""
	echo "    /* otherwise, we have to do a slow ${name}: */"
	echo "    else {"
	echo ""
	echo "      /* get the size of this bus cycle: */"
	echo "      count = (1 << TME_BUS${i_width}_LOG2);"
	echo "      count -= (address_init & (count - 1));"
	echo "      count = TME_MIN(count, size);"
	echo ""
	echo "      /* fill the cycle structure: */"
	echo "      cycle.tme_bus_cycle_type = TME_BUS_CYCLE_${capname};"
	echo "      cycle.tme_bus_cycle_size = count;"
	echo "      cycle.tme_bus_cycle_buffer = (tme_uint8_t *) buffer; /* XXX this breaks const */"
	echo "      cycle.tme_bus_cycle_buffer_increment = 1;"
	echo "      cycle.tme_bus_cycle_lane_routing"
	echo "        = (bus_device->tme_bus_device_router"
	echo "           + TME_BUS_ROUTER_INIT_INDEX(TME_BUS${i_width}_LOG2, count, address_init));"
	echo ""
	echo "      /* XXX this should come from a socket configuration: */"
	echo "      cycle.tme_bus_cycle_port = TME_BUS_CYCLE_PORT(0, TME_BUS${i_width}_LOG2);"
	echo ""
	echo "      /* form the physical address for the bus cycle handler: */"
	echo "      address_resp = tlb->tme_bus_tlb_addr_offset + address_init;"
	echo "      shift = tlb->tme_bus_tlb_addr_shift;"
	echo "      if (shift < 0) {"
	echo "        address_resp <<= (0 - shift);"
	echo "      }"
	echo "      else if (shift > 0) {"
	echo "        address_resp >>= shift;"
	echo "      }"
	echo "      cycle.tme_bus_cycle_address = address_resp;"
	echo ""
	echo "      /* unbusy this TLB entry: */"
	echo "      tme_bus_tlb_unbusy(tlb);"
	echo ""
	echo "      /* unlock the device: */"
	echo "      (*bus_device->tme_bus_device_unlock)(bus_device, locks);"
	echo ""
	echo "      /* run the bus cycle: */"
	echo "      err = (*tlb->tme_bus_tlb_cycle)"
	echo "           (tlb->tme_bus_tlb_cycle_private, &cycle);"
	echo ""
	echo "      /* if the TLB entry was invalidated before the ${name}: */"
	echo "      if (err == EBADF"
	echo "          && tme_bus_tlb_is_invalid(tlb)) {"
	echo "        count = 0;"
	echo "      }"
	echo ""
	echo "      /* otherwise, any other error might be a bus error: */"
	echo "      else if (err != TME_OK) {"
	echo "        err = tme_bus_tlb_fault(tlb, &cycle, err);"
	echo "        assert (err != TME_OK);"
	echo "      }"
	echo ""
	echo "      /* lock the device: */"
	echo "      (*bus_device->tme_bus_device_lock)(bus_device, locks);"
	echo "    }"
	echo ""
	echo "    /* update the address, buffer, and size and continue: */"
	echo "    address_init += count;"
	echo "    buffer += count;"
	echo "    size -= count;"
	echo "  }"
	echo ""
	echo "  return (err);"
	echo "}"
    done

done

# done:
#
exit 0		
