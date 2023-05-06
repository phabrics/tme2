AS_INIT

# $Id: m68k-bus-auto.sh,v 1.3 2007/02/12 23:47:11 fredette Exp $

# ic/m68k/m68k-bus-auto.sh - automatically generates C code 
# for m68k bus emulation support:

#
# Copyright (c) 2003 Matt Fredette
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
_TME_RCSID("\$Id: m68k-bus-auto.sh,v 1.3 2007/02/12 23:47:11 fredette Exp $");

/* we use OP3, OP2, OP1, and OP0 to represent bytes of lesser
   significance to more significance, respectively, matching Table 5-5
   in the MC68020 User's Manual (linear page 56 in my .ps copy).

   the Motorola OPn convention numbers bytes by decreasing
   significance (OP2 is less significant than OP1), and since Motorola
   CPUs are big-endian, this means that a higher numbered byte is
   meant to go to a higher address, which is good, because we can then
   use this to easily form indexes for TME_BUS_LANE_ROUTE, which
   expects a higher numbered index to correspond to a higher address
   in memory.

   however, since the same Motorola OPn convention always calls the
   least significant byte of any value OP3, regardless of the total
   size of the value, we need to adjust each OPn given the total
   size of the value, so that OP3 in a 24-bit value means address + 2,
   but OP3 in a 32-bit value means address + 3: */
#define SIZ8_OP(n)	((n) - 3)
#define SIZ16_OP(n)	((n) - 2)
#define SIZ24_OP(n)	((n) - 1)
#define SIZ32_OP(n)	((n) - 0)
EOF

# emit the 16-bit bus router:
if $header; then :; else
    AS_ECHO([""])
    AS_ECHO(["/* the 16-bit bus router used on the 68000 and 68010: */"])
    AS_ECHO(["const tme_bus_lane_t tme_m68k_router_16[[TME_M68K_BUS_ROUTER_SIZE(TME_BUS16_LOG2)]] = {"])

    # permute over maximum cycle size:
    for transfer in 1 2; do

	# permute over A0:
	for address in 0 1; do
	    address_bits=$address

	    # permute over the size of the responding device's port:
	    for port_size in 1 2; do

		# permute over the byte lane position of the responding device's port:
		for port_pos in 0 1; do

		    # get a string describing the byte lanes driven by
		    # this device:
		    port_pos_end=`expr ${port_pos} + ${port_size}`
		    port_pos_lane=$port_pos
		    port_lanes=""
		    while test `expr ${port_pos_lane} \< ${port_pos_end}` = 1; do
			port_lanes=" D"`expr \( \( ${port_pos_lane} + 1 \) \* 8 \) - 1`"-D"`expr ${port_pos_lane} \* 8`"${port_lanes}"
			port_pos_lane=`expr ${port_pos_lane} + 1`
		    done
		    if test `expr ${port_pos_end} \> 2` = 1; then
			port_lanes="${port_lanes} - invalid, array placeholder"
		    elif test $transfer = 1 \
		         && test $port_size = 1 \
		         && test $port_pos != `expr 1 - ${address}`; then
		        port_lanes="${port_lanes} - incorrect for 16-bit m68k"
		    fi

		    AS_ECHO([""])
		    AS_ECHO(["  /* [[m68k]] initiator maximum cycle size: "`expr ${transfer} \* 8`" bits"])
		    AS_ECHO(["     [[m68k]] initiator A0: ${address_bits}"])
		    AS_ECHO(["     [[gen]]  responding port size: "`expr ${port_size} \* 8`" bits"])
		    AS_ECHO(["     [[gen]]  responding port least lane: ${port_pos} (lanes${port_lanes})"])
		    AS_ECHO(["     (code ${transfer}.${address}.${port_size}.${port_pos}): */"])

		    # emit the bus router information for each lane:
		    for lane in 0 1; do

			# dispatch:
			lane_warn=
			case "${transfer}:${address}:${lane}" in
			2:0:1)
			    lane_read="OP(2)"
			    lane_write="OP(2)"
			    ;;
			2:0:0)
			    lane_read="OP(3)"
			    lane_write="OP(3)"
			    ;;
			2:1:[[01]])
			    lane_read="ABORT"
			    lane_write="ABORT"
			    ;;
			1:0:1 | 1:1:0)
			    lane_read="OP(3)"
			    lane_write="OP(3)"
			    ;;
			1:0:0 | 1:1:1)
			    lane_read="IGNORE"
			    lane_write="OP(3)"
			    if test $port_size = 1 \
			       && test $port_pos != `expr 1 - ${address}`; then
				lane_warn=" | TME_BUS_LANE_WARN"
			    fi
			    ;;
			*)
			    AS_ECHO(["$PROG internal error: unhandled 16-bit bus case ${transfer}:${address}:${lane}"]) 1>&2
			    exit 1
			    ;;
			esac

			# emit the comment for this lane:
			AS_ECHO_N(["  /* D"`expr \( \( ${lane} + 1 \) \* 8 \) - 1`"-D"`expr ${lane} \* 8`" */	"])

			# if this port size/position combination is
			# invalid, override everything and abort if
			# this router entry is ever touched:
			if test `expr ${port_pos_end} \> 2` = 1; then
			    AS_ECHO(["TME_BUS_LANE_ABORT,"])
			else
			    if test $lane_read = "ABORT"; then
				AS_ECHO_N(["TME_BUS_LANE_ABORT"])
			    elif test $lane_read != "IGNORE"; then
				if test $lane_read != $lane_write; then
				    AS_ECHO(["$PROG internal error: code ${transfer}:${address}:${lane}, reading $lane_read but writing $lane_write"]) 1>&2
				    exit 1
				fi
				AS_ECHO_N(["TME_BUS_LANE_ROUTE(SIZ"`expr ${transfer} \* 8`"_$lane_read)"])
			    else
				AS_ECHO_N(["TME_BUS_LANE_ROUTE(SIZ"`expr ${transfer} \* 8`"_$lane_write) | TME_BUS_LANE_ROUTE_WRITE_IGNORE"])
			    fi
			    AS_ECHO(["${lane_warn},"])
			fi
		    done
		done
	    done
	done
   done
   AS_ECHO(["};"])
fi

# emit the 32-bit bus router:
if $header; then :; else
    AS_ECHO([""])
    AS_ECHO(["/* the 32-bit bus router used on the 68020 and 68030: */"])
    AS_ECHO(["const tme_bus_lane_t tme_m68k_router_32[[TME_M68K_BUS_ROUTER_SIZE(TME_BUS32_LOG2)]] = {"])

    # permute over maximum cycle size:
    for transfer in 1 2 3 4; do
	# these are real 68020 SIZ1 and SIZ0 bits:
	case ${transfer} in
	1) transfer_bits="01" ;;
	2) transfer_bits="10" ;;
	3) transfer_bits="11" ;;
	4) transfer_bits="00" ;;
	esac

	# permute over A1 and A0:
	for address in 0 1 2 3; do
	    case $address in
	    0) address_bits=00 ;;
	    1) address_bits=01 ;;
	    2) address_bits=10 ;;
	    3) address_bits=11 ;;
	    esac

	    # permute over the size of the responding device's port:
	    for port_size in 1 2 4; do

		# permute over the byte lane position of the responding device's port:
		for port_pos in 0 1 2 3; do

		    # get a string describing the byte lanes connected
		    # to this device.  NB that the m68k 32-bit bus
		    # router assumes that 8-bit devices are always
		    # connected to D31-D24, and that 16-bit devices
		    # are always connected to D31-D24 and D23-D16, and
		    # cannot dynamically adapt to other
		    # configurations:
		    port_pos_end=`expr ${port_pos} + ${port_size}`
		    port_pos_lane=$port_pos
		    port_lanes=""
		    while test `expr ${port_pos_lane} \< ${port_pos_end}` = 1; do
			port_lanes=" D"`expr \( \( ${port_pos_lane} + 1 \) \* 8 \) - 1`"-D"`expr ${port_pos_lane} \* 8`"${port_lanes}"
			port_pos_lane=`expr ${port_pos_lane} + 1`
		    done
		    if test `expr ${port_pos_end} \> 4` = 1; then
			port_lanes="${port_lanes} - invalid, array placeholder"
		    elif ( test $port_size = 1 && test $port_pos != 3 ) \
		         || ( test $port_size = 2 && test $port_pos != 2 ); then
		        port_lanes="${port_lanes} - incorrect for 32-bit m68k"
		    fi

		    # find the byte lane that would provide OP3.  note
		    # that it may not exist (lane < 0), or that it may
		    # not be within the port:
		    opn=`expr 4 - ${transfer}`
		    opn_lane=`expr 3 - \( ${address} % ${port_size} \)`
		    op3_lane=`expr \( ${opn_lane} \) - \( 3 - ${opn} \)`

		    AS_ECHO([""])
		    AS_ECHO(["  /* [[m68k]] initiator maximum cycle size: "`expr ${transfer} \* 8`" bits"])
		    AS_ECHO(["     [[m68k]] initiator A1,A0: ${address_bits}"])
		    AS_ECHO(["     [[gen]]  responder port size: "`expr ${port_size} \* 8`" bits"])
		    AS_ECHO(["     [[gen]]  responder port least lane: ${port_pos} (lanes${port_lanes})"])
		    AS_ECHO(["     (code ${transfer}.${address}.${port_size}.${port_pos}, OP3 lane ${op3_lane}): */"])

		    # emit the bus router information for each lane:
		    for lane in 0 1 2 3; do
			lane_warn=

			# if the m68k expects this byte lane to be connected 
			# to the device at this port size:
			if test `expr ${lane} \< \( 4 - ${port_size} \)` = 0; then

			    # if this lane is routed by the m68k when
			    # reading at this transfer size and
			    # address alignment:
			    opn=`expr 3 - \( ${lane} - ${op3_lane} \)`
			    if test `expr \( ${opn} \) \> 3` = 0 \
			       && test `expr \( ${opn} \) \< \( 4 - ${transfer} \)` = 0; then
				lane_read="OP(${opn})"

			    # otherwise this lane isn't routed by the
			    # m68k when reading at this transfer size
			    # and address alignment:
			    else
				lane_read="IGNORE"
			    fi

			# otherwise the m68k does not expect this byte
			# lane to be connected to the device at this
			# port size:
			else
			    lane_read="IGNORE"

			    # if this lane is connected to the device
			    # anyways, issue a warning:
			    if test `expr ${lane} \< ${port_pos}` = 0 \
			       && test `expr ${lane} \< ${port_pos_end}` = 1; then
				lane_warn=" | TME_BUS_LANE_WARN"
			    fi
			fi

			# dispatch on how this lane is routed by the
			# m68k when writing at this transfer size and
			# address alignment:
			case "${transfer_bits}${address_bits}.${lane}" in
			01??.3) lane_write="OP(3)" ;;
			01??.2) lane_write="OP(3)" ;;
			01??.1) lane_write="OP(3)" ;;
			01??.0) lane_write="OP(3)" ;;

			10?0.3) lane_write="OP(2)" ;;
			10?0.2) lane_write="OP(3)" ;;
			10?0.1) lane_write="OP(2)" ;;
			10?0.0) lane_write="OP(3)" ;;
			
			10?1.3) lane_write="OP(2)" ;;
			10?1.2) lane_write="OP(2)" ;;
			10?1.1) lane_write="OP(3)" ;;
			10?1.0) lane_write="OP(2)" ;;

			1100.3) lane_write="OP(1)" ;;
			1100.2) lane_write="OP(2)" ;;
			1100.1) lane_write="OP(3)" ;;
			1100.0) lane_write="UNDEF" ;; # XXX this is supposed to be OP0, but we can't deal with that

			1101.3) lane_write="OP(1)" ;;
			1101.2) lane_write="OP(1)" ;;
			1101.1) lane_write="OP(2)" ;;
			1101.0) lane_write="OP(3)" ;;

			1110.3) lane_write="OP(1)" ;;
			1110.2) lane_write="OP(2)" ;;
			1110.1) lane_write="OP(1)" ;;
			1110.0) lane_write="OP(2)" ;;

			1111.3) lane_write="OP(1)" ;;
			1111.2) lane_write="OP(1)" ;;
			1111.1) lane_write="OP(2)" ;;
			1111.0) lane_write="OP(1)" ;;

			0000.3) lane_write="OP(0)" ;;
			0000.2) lane_write="OP(1)" ;;
			0000.1) lane_write="OP(2)" ;;
			0000.0) lane_write="OP(3)" ;;

			0001.3) lane_write="OP(0)" ;;
			0001.2) lane_write="OP(0)" ;;
			0001.1) lane_write="OP(1)" ;;
			0001.0) lane_write="OP(2)" ;;

			0010.3) lane_write="OP(0)" ;;
			0010.2) lane_write="OP(1)" ;;
			0010.1) lane_write="OP(0)" ;;
			0010.0) lane_write="OP(1)" ;;

			0011.3) lane_write="OP(0)" ;;
			0011.2) lane_write="OP(0)" ;;
			0011.1) lane_write="OP(1)" ;;
			0011.0) lane_write="OP(0)" ;;
			
			esac

			# emit the comment for this lane:
			AS_ECHO_N(["  /* D"`expr \( \( ${lane} + 1 \) \* 8 \) - 1`"-D"`expr ${lane} \* 8`" */	"])

			# if this port size/position combination is
			# invalid, override everything and abort if
			# this router entry is ever touched:
			if test `expr ${port_pos_end} \> 4` = 1; then
			    AS_ECHO(["TME_BUS_LANE_ABORT,"])
			else
			    if test $lane_read != "IGNORE"; then
				if test $lane_read != $lane_write; then
				    AS_ECHO(["$PROG internal error: code ${transfer}.${address}.${port_size}.${port_pos}, reading $lane_read but writing $lane_write" 1>&2])
				    exit 1
				fi
				AS_ECHO_N(["TME_BUS_LANE_ROUTE(SIZ"`expr ${transfer} \* 8`"_$lane_read)"])
			    elif test $lane_write = "UNDEF"; then
				AS_ECHO_N(["TME_BUS_LANE_UNDEF"])
			    else
				AS_ECHO_N(["TME_BUS_LANE_ROUTE(SIZ"`expr ${transfer} \* 8`"_$lane_write) | TME_BUS_LANE_ROUTE_WRITE_IGNORE"])
			    fi
			    AS_ECHO(["${lane_warn},"])
			fi
		    done
		done
	    done
	done
   done
   AS_ECHO(["};"])
fi

cat <<EOF
#undef SIZ8_OP
#undef SIZ16_OP
#undef SIZ24_OP
#undef SIZ32_OP
EOF

# done:
exit 0
