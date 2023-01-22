/* automatically generated by sparc-fpu-auto.sh, do not edit! */
_TME_RCSID("$Id: sparc-fpu-auto.sh,v 1.6 2009/11/08 16:27:12 fredette Exp $");

/* this sets the floating-point condition codes after a
   single-precision operation: */
static inline void
_tme_sparc_fpu_fcc_single(struct tme_sparc *ic, const struct tme_float *dst, int trap_on_nan)
{
  tme_uint32_t fcc;
  unsigned int cc;

  /* set fcc: */
  fcc = (tme_float_is_nan(dst, (TME_FLOAT_FORMAT_IEEE754_SINGLE | TME_FLOAT_FORMAT_IEEE754_SINGLE_BUILTIN))
         ? TME_SPARC_FSR_FCC_UN
	 : tme_float_is_zero(dst, (TME_FLOAT_FORMAT_IEEE754_SINGLE | TME_FLOAT_FORMAT_IEEE754_SINGLE_BUILTIN))
	 ? TME_SPARC_FSR_FCC_EQ
	 : tme_float_is_negative(dst, (TME_FLOAT_FORMAT_IEEE754_SINGLE | TME_FLOAT_FORMAT_IEEE754_SINGLE_BUILTIN))
	 ? TME_SPARC_FSR_FCC_LT
	 : TME_SPARC_FSR_FCC_GT);

  /* if this is an FCMPE and this is a NaN, we always cause an invalid exception: */
  if (trap_on_nan && fcc == TME_SPARC_FSR_FCC_UN) {
    _tme_sparc_fpu_exception_ieee754(&ic->tme_sparc_fpu_ieee754_ctl, TME_FLOAT_EXCEPTION_INVALID);
  }

  /* set the floating-point condition codes: */
  if (TME_SPARC_VERSION(ic) >= 9) {
    cc = TME_FIELD_MASK_EXTRACTU(TME_SPARC_INSN, (0x3 << 25));
    if (cc != 0) {
      fcc = TME_FIELD_MASK_EXTRACTU(fcc, TME_SPARC_FSR_FCC);
      ic->tme_sparc_fpu_xfsr
	= ((ic->tme_sparc_fpu_xfsr
	    & ~ (tme_uint32_t) (0x3 << (2 * (cc - 1))))
	   | (fcc << (2 * (cc - 1))));
      return;
    }
  }
  ic->tme_sparc_fpu_fsr = (ic->tme_sparc_fpu_fsr & ~TME_SPARC_FSR_FCC) | fcc;
}

/* if the most significant bit of the NaN fraction is zero,
   this is a signaling NaN: */
#define _TME_SPARC_FPU_IS_SNAN_SINGLE(a) (((*(a)) & ((0x007f0000 | (0x007f0000 >> 1)) ^ (0x007f0000 >> 1))) != 0)
static tme_int8_t
_tme_sparc_fpu_is_snan_single(tme_uint32_t *value)
{
  return (_TME_SPARC_FPU_IS_SNAN_SINGLE(value));
}

/* single-precision NaN propagation: */
static void
_tme_sparc_fpu_nan_from_nans_single(struct tme_ieee754_ctl *ctl,
				     const tme_uint32_t *a,
				     const tme_uint32_t *b,
				     tme_uint32_t *z)
{
  struct tme_sparc *ic;
  int a_is_snan;
  int b_is_snan;

  /* recover our data structure : */
  ic = ctl->tme_ieee754_ctl_private;

  /* see if any of the NaNs are signaling NaNs: */
  a_is_snan = _TME_SPARC_FPU_IS_SNAN_SINGLE(a);
  b_is_snan = _TME_SPARC_FPU_IS_SNAN_SINGLE(b);

  /* if either operand is a signaling NaN: */
  if (a_is_snan || b_is_snan) {

    /* signal the signaling NaN: */
    _tme_sparc_fpu_exception_ieee754(ctl, TME_FLOAT_EXCEPTION_INVALID);
  }

  /* if and only if a (corresponding to f[rs1]) is a signaling NaN, do
     we return a. at all other times we return b (corresponding to f[rs2]): */
  if (a_is_snan) {
    b = a;
  }

  /* return the NaN, but make sure it's nonsignaling: */
  *z = *b;
  (*(z)) |= ((0x007f0000 | (0x007f0000 >> 1)) ^ (0x007f0000 >> 1));
}


/* this sets the floating-point condition codes after a
   double-precision operation: */
static inline void
_tme_sparc_fpu_fcc_double(struct tme_sparc *ic, const struct tme_float *dst, int trap_on_nan)
{
  tme_uint32_t fcc;
  unsigned int cc;

  /* set fcc: */
  fcc = (tme_float_is_nan(dst, (TME_FLOAT_FORMAT_IEEE754_DOUBLE | TME_FLOAT_FORMAT_IEEE754_DOUBLE_BUILTIN))
         ? TME_SPARC_FSR_FCC_UN
	 : tme_float_is_zero(dst, (TME_FLOAT_FORMAT_IEEE754_DOUBLE | TME_FLOAT_FORMAT_IEEE754_DOUBLE_BUILTIN))
	 ? TME_SPARC_FSR_FCC_EQ
	 : tme_float_is_negative(dst, (TME_FLOAT_FORMAT_IEEE754_DOUBLE | TME_FLOAT_FORMAT_IEEE754_DOUBLE_BUILTIN))
	 ? TME_SPARC_FSR_FCC_LT
	 : TME_SPARC_FSR_FCC_GT);

  /* if this is an FCMPE and this is a NaN, we always cause an invalid exception: */
  if (trap_on_nan && fcc == TME_SPARC_FSR_FCC_UN) {
    _tme_sparc_fpu_exception_ieee754(&ic->tme_sparc_fpu_ieee754_ctl, TME_FLOAT_EXCEPTION_INVALID);
  }

  /* set the floating-point condition codes: */
  if (TME_SPARC_VERSION(ic) >= 9) {
    cc = TME_FIELD_MASK_EXTRACTU(TME_SPARC_INSN, (0x3 << 25));
    if (cc != 0) {
      fcc = TME_FIELD_MASK_EXTRACTU(fcc, TME_SPARC_FSR_FCC);
      ic->tme_sparc_fpu_xfsr
	= ((ic->tme_sparc_fpu_xfsr
	    & ~ (tme_uint32_t) (0x3 << (2 * (cc - 1))))
	   | (fcc << (2 * (cc - 1))));
      return;
    }
  }
  ic->tme_sparc_fpu_fsr = (ic->tme_sparc_fpu_fsr & ~TME_SPARC_FSR_FCC) | fcc;
}

/* if the most significant bit of the NaN fraction is zero,
   this is a signaling NaN: */
#define _TME_SPARC_FPU_IS_SNAN_DOUBLE(a) (((*(a)).tme_value64_uint32_hi & ((0x000f0000 | (0x000f0000 >> 1)) ^ (0x000f0000 >> 1))) != 0)
static tme_int8_t
_tme_sparc_fpu_is_snan_double(union tme_value64 *value)
{
  return (_TME_SPARC_FPU_IS_SNAN_DOUBLE(value));
}

/* double-precision NaN propagation: */
static void
_tme_sparc_fpu_nan_from_nans_double(struct tme_ieee754_ctl *ctl,
				     const union tme_value64 *a,
				     const union tme_value64 *b,
				     union tme_value64 *z)
{
  struct tme_sparc *ic;
  int a_is_snan;
  int b_is_snan;

  /* recover our data structure : */
  ic = ctl->tme_ieee754_ctl_private;

  /* see if any of the NaNs are signaling NaNs: */
  a_is_snan = _TME_SPARC_FPU_IS_SNAN_DOUBLE(a);
  b_is_snan = _TME_SPARC_FPU_IS_SNAN_DOUBLE(b);

  /* if either operand is a signaling NaN: */
  if (a_is_snan || b_is_snan) {

    /* signal the signaling NaN: */
    _tme_sparc_fpu_exception_ieee754(ctl, TME_FLOAT_EXCEPTION_INVALID);
  }

  /* if and only if a (corresponding to f[rs1]) is a signaling NaN, do
     we return a. at all other times we return b (corresponding to f[rs2]): */
  if (a_is_snan) {
    b = a;
  }

  /* return the NaN, but make sure it's nonsignaling: */
  *z = *b;
  (*(z)).tme_value64_uint32_hi |= ((0x000f0000 | (0x000f0000 >> 1)) ^ (0x000f0000 >> 1));
}


/* this sets the floating-point condition codes after a
   quad-precision operation: */
static inline void
_tme_sparc_fpu_fcc_quad(struct tme_sparc *ic, const struct tme_float *dst, int trap_on_nan)
{
  tme_uint32_t fcc;
  unsigned int cc;

  /* set fcc: */
  fcc = (tme_float_is_nan(dst, (TME_FLOAT_FORMAT_IEEE754_QUAD | TME_FLOAT_FORMAT_IEEE754_QUAD_BUILTIN))
         ? TME_SPARC_FSR_FCC_UN
	 : tme_float_is_zero(dst, (TME_FLOAT_FORMAT_IEEE754_QUAD | TME_FLOAT_FORMAT_IEEE754_QUAD_BUILTIN))
	 ? TME_SPARC_FSR_FCC_EQ
	 : tme_float_is_negative(dst, (TME_FLOAT_FORMAT_IEEE754_QUAD | TME_FLOAT_FORMAT_IEEE754_QUAD_BUILTIN))
	 ? TME_SPARC_FSR_FCC_LT
	 : TME_SPARC_FSR_FCC_GT);

  /* if this is an FCMPE and this is a NaN, we always cause an invalid exception: */
  if (trap_on_nan && fcc == TME_SPARC_FSR_FCC_UN) {
    _tme_sparc_fpu_exception_ieee754(&ic->tme_sparc_fpu_ieee754_ctl, TME_FLOAT_EXCEPTION_INVALID);
  }

  /* set the floating-point condition codes: */
  if (TME_SPARC_VERSION(ic) >= 9) {
    cc = TME_FIELD_MASK_EXTRACTU(TME_SPARC_INSN, (0x3 << 25));
    if (cc != 0) {
      fcc = TME_FIELD_MASK_EXTRACTU(fcc, TME_SPARC_FSR_FCC);
      ic->tme_sparc_fpu_xfsr
	= ((ic->tme_sparc_fpu_xfsr
	    & ~ (tme_uint32_t) (0x3 << (2 * (cc - 1))))
	   | (fcc << (2 * (cc - 1))));
      return;
    }
  }
  ic->tme_sparc_fpu_fsr = (ic->tme_sparc_fpu_fsr & ~TME_SPARC_FSR_FCC) | fcc;
}

/* if the most significant bit of the NaN fraction is zero,
   this is a signaling NaN: */
#define _TME_SPARC_FPU_IS_SNAN_QUAD(a) (((*(a)).tme_float_ieee754_quad_hi.tme_value64_uint32_hi & ((0x0000ffff | (0x0000ffff >> 1)) ^ (0x0000ffff >> 1))) != 0)
static tme_int8_t
_tme_sparc_fpu_is_snan_quad(struct tme_float_ieee754_quad *value)
{
  return (_TME_SPARC_FPU_IS_SNAN_QUAD(value));
}

/* quad-precision NaN propagation: */
static void
_tme_sparc_fpu_nan_from_nans_quad(struct tme_ieee754_ctl *ctl,
				     const struct tme_float_ieee754_quad *a,
				     const struct tme_float_ieee754_quad *b,
				     struct tme_float_ieee754_quad *z)
{
  struct tme_sparc *ic;
  int a_is_snan;
  int b_is_snan;

  /* recover our data structure : */
  ic = ctl->tme_ieee754_ctl_private;

  /* see if any of the NaNs are signaling NaNs: */
  a_is_snan = _TME_SPARC_FPU_IS_SNAN_QUAD(a);
  b_is_snan = _TME_SPARC_FPU_IS_SNAN_QUAD(b);

  /* if either operand is a signaling NaN: */
  if (a_is_snan || b_is_snan) {

    /* signal the signaling NaN: */
    _tme_sparc_fpu_exception_ieee754(ctl, TME_FLOAT_EXCEPTION_INVALID);
  }

  /* if and only if a (corresponding to f[rs1]) is a signaling NaN, do
     we return a. at all other times we return b (corresponding to f[rs2]): */
  if (a_is_snan) {
    b = a;
  }

  /* return the NaN, but make sure it's nonsignaling: */
  *z = *b;
  (*(z)).tme_float_ieee754_quad_hi.tme_value64_uint32_hi |= ((0x0000ffff | (0x0000ffff >> 1)) ^ (0x0000ffff >> 1));
}

#define _TME_SPARC_FPU_UNIMPL tme_sparc_fpu_exception(ic, TME_SPARC_FSR_FTT_unimplemented_FPop)
#define _TME_SPARC_FPU_UNIMPL_IF(flags) do { if ((ic->tme_sparc_fpu_flags & (flags)) != 0) { _TME_SPARC_FPU_UNIMPL; } } while (/* CONSTCOND */ 0)

void
tme_sparc_fpu_fpop1(struct tme_sparc *ic)
{
  tme_uint8_t rounding_mode;
  unsigned int opf;
  unsigned int fpreg_rd_number_encoded;
  unsigned int fpreg_rd_number;
  const struct tme_float *fpreg_rs1;
  const struct tme_float *fpreg_rs2;
  struct tme_float fpreg_rs1_buffer;
  struct tme_float fpreg_rs2_buffer;
  struct tme_float fpreg_rd;
  unsigned int fpreg_rd_format;

  /* set the rounding mode: */
  switch (ic->tme_sparc_fpu_fsr & TME_SPARC_FSR_RND) {
  default: assert(FALSE);
  case TME_SPARC_FSR_RND_RN: rounding_mode = TME_FLOAT_ROUND_NEAREST_EVEN; break;
  case TME_SPARC_FSR_RND_RZ: rounding_mode = TME_FLOAT_ROUND_TO_ZERO; break;
  case TME_SPARC_FSR_RND_RM: rounding_mode = TME_FLOAT_ROUND_DOWN; break;
  case TME_SPARC_FSR_RND_RP: rounding_mode = TME_FLOAT_ROUND_UP; break;
  }
  ic->tme_sparc_fpu_ieee754_ctl.tme_ieee754_ctl_rounding_mode = rounding_mode;

  /* decode the rd and opf fields: */
  fpreg_rd_number_encoded = TME_FIELD_MASK_EXTRACTU(TME_SPARC_INSN, TME_SPARC_FORMAT3_MASK_RD);
  opf = TME_FIELD_MASK_EXTRACTU(TME_SPARC_INSN, (0x1ff << 5));

  /* silence uninitialized variable warnings: */
  fpreg_rd_number = 0;

#ifdef _TME_SPARC_RECODE_VERIFY
  /* clear the rd buffer: */
  memset(&fpreg_rd, 0, sizeof(fpreg_rd));
#endif /* _TME_SPARC_RECODE_VERIFY */

  /* dispatch on the opf field: */
  switch (opf) {
#define _TME_SPARC_FPU_FORMAT_RS1(format) fpreg_rs1 = tme_sparc_fpu_fpreg_read(ic, TME_SPARC_FORMAT3_MASK_RS1, (format))
#define _TME_SPARC_FPU_FORMAT_RS2(format) fpreg_rs2 = tme_sparc_fpu_fpreg_read(ic, TME_SPARC_FORMAT3_MASK_RS2, (format))
#define _TME_SPARC_FPU_FORMAT_RD(format) do { fpreg_rd_format = (format) | TME_IEEE754_FPREG_FORMAT_BUILTIN; fpreg_rd_number = tme_sparc_fpu_fpreg_decode(ic, fpreg_rd_number_encoded, fpreg_rd_format); } while (/* CONSTCOND */ 0)

  case 1:  /* 000000001 FMOVs: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd = *fpreg_rs2;
    break;

  case 2:  /* 000000010 FMOVd: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd = *fpreg_rs2;
    break;

  case 5:  /* 000000101 FNEGs: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd = *fpreg_rs2;
    fpreg_rd.tme_float_value_ieee754_single ^= 0x80000000;
    break;

  case 6:  /* 000000110 FNEGd: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd = *fpreg_rs2;
    fpreg_rd.tme_float_value_ieee754_double.tme_value64_uint32_hi ^= 0x80000000;
    break;

  case 9:  /* 000001001 FABSs: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd = *fpreg_rs2;
    fpreg_rd.tme_float_value_ieee754_single &= ~0x80000000;
    break;

  case 10:  /* 000001010 FABSd: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd = *fpreg_rs2;
    fpreg_rd.tme_float_value_ieee754_double.tme_value64_uint32_hi &= ~0x80000000;
    break;

  case 41:  /* 000101001 FSQRTs: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_FSQRT);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_single_sqrt,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 42:  /* 000101010 FSQRTd: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_FSQRT);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_sqrt,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 43:  /* 000101011 FSQRTq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_FSQRT | TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_sqrt,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 65:  /* 001000001 FADDs: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_single_add,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 66:  /* 001000010 FADDd: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_double_add,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 67:  /* 001000011 FADDq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_quad_add,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 69:  /* 001000101 FSUBs: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_single_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 70:  /* 001000110 FSUBd: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_double_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 71:  /* 001000111 FSUBq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_quad_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 73:  /* 001001001 FMULs: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_single_mul,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 74:  /* 001001010 FMULd: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_double_mul,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 75:  /* 001001011 FMULq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_quad_mul,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 77:  /* 001001101 FDIVs: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_single_div,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 78:  /* 001001110 FDIVd: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_double_div,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 79:  /* 001001111 FDIVq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_quad_div,
                              fpreg_rs1, fpreg_rs2, &fpreg_rd);
    break;

  case 105:  /* 001101001 FsMULd: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_FMUL_WIDER);
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_from_single,
                              fpreg_rs1, &fpreg_rs1_buffer);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_from_single,
                              fpreg_rs2, &fpreg_rs2_buffer);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_double_mul,
                             &fpreg_rs1_buffer, &fpreg_rs2_buffer, &fpreg_rd);
    break;

  case 110:  /* 001101110 FdMULq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_FMUL_WIDER | TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_from_double,
                              fpreg_rs1, &fpreg_rs1_buffer);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_from_double,
                              fpreg_rs2, &fpreg_rs2_buffer);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_quad_mul,
                             &fpreg_rs1_buffer, &fpreg_rs2_buffer, &fpreg_rd);
    break;

  case 129:  /* 010000001 FsTOx: */
#ifdef TME_HAVE_INT64_T
    if (__tme_predict_true(TME_SPARC_VERSION(ic) >= 9)) {
      _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
      _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
      _TME_SPARC_FPU_BEGIN;
      fpreg_rd.tme_float_format = TME_FLOAT_FORMAT_IEEE754_DOUBLE;
      ic->tme_sparc_fpu_ieee754_ctl.tme_ieee754_ctl_rounding_mode = TME_FLOAT_ROUND_TO_ZERO;
      _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_single_to_int64,
                                fpreg_rs2, &fpreg_rd.tme_float_value_ieee754_double.tme_value64_int);
      break;
    }
#endif /* TME_HAVE_INT64_T */
    _TME_SPARC_FPU_UNIMPL;
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 130:  /* 010000010 FdTOx: */
#ifdef TME_HAVE_INT64_T
    if (__tme_predict_true(TME_SPARC_VERSION(ic) >= 9)) {
      _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
      _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
      _TME_SPARC_FPU_BEGIN;
      fpreg_rd.tme_float_format = TME_FLOAT_FORMAT_IEEE754_DOUBLE;
      ic->tme_sparc_fpu_ieee754_ctl.tme_ieee754_ctl_rounding_mode = TME_FLOAT_ROUND_TO_ZERO;
      _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_to_int64,
                                fpreg_rs2, &fpreg_rd.tme_float_value_ieee754_double.tme_value64_int);
      break;
    }
#endif /* TME_HAVE_INT64_T */
    _TME_SPARC_FPU_UNIMPL;
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 131:  /* 010000011 FqTOx: */
#ifdef TME_HAVE_INT64_T
    if (__tme_predict_true(TME_SPARC_VERSION(ic) >= 9)) {
      _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
      _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
      _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
      _TME_SPARC_FPU_BEGIN;
      fpreg_rd.tme_float_format = TME_FLOAT_FORMAT_IEEE754_DOUBLE;
      ic->tme_sparc_fpu_ieee754_ctl.tme_ieee754_ctl_rounding_mode = TME_FLOAT_ROUND_TO_ZERO;
      _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_to_int64,
                                fpreg_rs2, &fpreg_rd.tme_float_value_ieee754_double.tme_value64_int);
      break;
    }
#endif /* TME_HAVE_INT64_T */
    _TME_SPARC_FPU_UNIMPL;
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 132:  /* 010000100 FxTOs: */
#ifdef TME_HAVE_INT64_T
    if (__tme_predict_true(TME_SPARC_VERSION(ic) >= 9)) {
      _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE);
      _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
      _TME_SPARC_FPU_BEGIN;
      _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_single_from_int64,
                                fpreg_rs2->tme_float_value_ieee754_double.tme_value64_int, &fpreg_rd);
      break;
    }
#endif /* TME_HAVE_INT64_T */
    _TME_SPARC_FPU_UNIMPL;
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 136:  /* 010001000 FxTOd: */
#ifdef TME_HAVE_INT64_T
    if (__tme_predict_true(TME_SPARC_VERSION(ic) >= 9)) {
      _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE);
      _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
      _TME_SPARC_FPU_BEGIN;
      _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_from_int64,
                                fpreg_rs2->tme_float_value_ieee754_double.tme_value64_int, &fpreg_rd);
      break;
    }
#endif /* TME_HAVE_INT64_T */
    _TME_SPARC_FPU_UNIMPL;
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 140:  /* 010001100 FxTOq: */
#ifdef TME_HAVE_INT64_T
    if (__tme_predict_true(TME_SPARC_VERSION(ic) >= 9)) {
      _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
      _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE);
      _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
      _TME_SPARC_FPU_BEGIN;
      _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_from_int64,
                                fpreg_rs2->tme_float_value_ieee754_double.tme_value64_int, &fpreg_rd);
      break;
    }
#endif /* TME_HAVE_INT64_T */
    _TME_SPARC_FPU_UNIMPL;
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 196:  /* 011000100 FiTOs: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_BEGIN;
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_single_from_int32,
                              fpreg_rs2->tme_float_value_ieee754_single, &fpreg_rd);
    break;

  case 198:  /* 011000110 FdTOs: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_single_from_double,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 199:  /* 011000111 FqTOs: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_single_from_quad,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 200:  /* 011001000 FiTOd: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_BEGIN;
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_from_int32,
                              fpreg_rs2->tme_float_value_ieee754_single, &fpreg_rd);
    break;

  case 201:  /* 011001001 FsTOd: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_from_single,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 203:  /* 011001011 FqTOd: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_from_quad,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 204:  /* 011001100 FiTOq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_BEGIN;
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_from_int32,
                              fpreg_rs2->tme_float_value_ieee754_single, &fpreg_rd);
    break;

  case 205:  /* 011001101 FsTOq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_from_single,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 206:  /* 011001110 FdTOq: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_QUAD);
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_from_double,
                              fpreg_rs2, &fpreg_rd);
    break;

  case 209:  /* 011010001 FsTOi: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd.tme_float_format = TME_FLOAT_FORMAT_IEEE754_SINGLE;
    ic->tme_sparc_fpu_ieee754_ctl.tme_ieee754_ctl_rounding_mode = TME_FLOAT_ROUND_TO_ZERO;
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_single_to_int32,
                              fpreg_rs2, (tme_int32_t *) &fpreg_rd.tme_float_value_ieee754_single);
    break;

  case 210:  /* 011010010 FdTOi: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd.tme_float_format = TME_FLOAT_FORMAT_IEEE754_SINGLE;
    ic->tme_sparc_fpu_ieee754_ctl.tme_ieee754_ctl_rounding_mode = TME_FLOAT_ROUND_TO_ZERO;
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_double_to_int32,
                              fpreg_rs2, (tme_int32_t *) &fpreg_rd.tme_float_value_ieee754_single);
    break;

  case 211:  /* 011010011 FqTOi: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd.tme_float_format = TME_FLOAT_FORMAT_IEEE754_SINGLE;
    ic->tme_sparc_fpu_ieee754_ctl.tme_ieee754_ctl_rounding_mode = TME_FLOAT_ROUND_TO_ZERO;
    _TME_SPARC_FPU_OP_MONADIC(tme_ieee754_ops_quad_to_int32,
                              fpreg_rs2, (tme_int32_t *) &fpreg_rd.tme_float_value_ieee754_single);
    break;

  default:
    _TME_SPARC_FPU_UNIMPL;
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

#undef _TME_SPARC_FPU_FORMAT_RS1
#undef _TME_SPARC_FPU_FORMAT_RS2
#undef _TME_SPARC_FPU_FORMAT_RD
  }

  /* store any destination: */
  if (fpreg_rd_format != TME_IEEE754_FPREG_FORMAT_NULL) {
    tme_sparc_fpu_fpreg_format(ic, fpreg_rd_number, fpreg_rd_format);
    ic->tme_sparc_fpu_fpregs[fpreg_rd_number] = fpreg_rd;
    TME_SPARC_FPU_DIRTY(ic, fpreg_rd_number);
  }

}

void
tme_sparc_fpu_fpop2(struct tme_sparc *ic)
{
  tme_uint8_t rounding_mode;
  unsigned int opf;
  unsigned int fpreg_rd_number_encoded;
  unsigned int fpreg_rd_number;
  const struct tme_float *fpreg_rs1;
  const struct tme_float *fpreg_rs2;
  unsigned int cc;
  tme_uint32_t conds_mask;
  unsigned int cc_i;
  tme_uint32_t cond;
  struct tme_float fpreg_rd;
  unsigned int fpreg_rd_format;

  /* set the rounding mode: */
  switch (ic->tme_sparc_fpu_fsr & TME_SPARC_FSR_RND) {
  default: assert(FALSE);
  case TME_SPARC_FSR_RND_RN: rounding_mode = TME_FLOAT_ROUND_NEAREST_EVEN; break;
  case TME_SPARC_FSR_RND_RZ: rounding_mode = TME_FLOAT_ROUND_TO_ZERO; break;
  case TME_SPARC_FSR_RND_RM: rounding_mode = TME_FLOAT_ROUND_DOWN; break;
  case TME_SPARC_FSR_RND_RP: rounding_mode = TME_FLOAT_ROUND_UP; break;
  }
  ic->tme_sparc_fpu_ieee754_ctl.tme_ieee754_ctl_rounding_mode = rounding_mode;

  /* decode the rd and opf fields: */
  fpreg_rd_number_encoded = TME_FIELD_MASK_EXTRACTU(TME_SPARC_INSN, TME_SPARC_FORMAT3_MASK_RD);
  opf = TME_FIELD_MASK_EXTRACTU(TME_SPARC_INSN, (0x1ff << 5));

  /* silence uninitialized variable warnings: */
  fpreg_rd_number = 0;

#ifdef _TME_SPARC_RECODE_VERIFY
  /* clear the rd buffer: */
  memset(&fpreg_rd, 0, sizeof(fpreg_rd));
#endif /* _TME_SPARC_RECODE_VERIFY */

  /* if this is an FMOVcc: */
  if (((opf - 1) & 0x3f) < 3) {

    /* if opf bit eight is set, this uses integer condition codes: */
    if (opf & TME_BIT(8)) {

      /* if opf bit six is set, this is unimplemented: */
      if (__tme_predict_false(opf & TME_BIT(6))) {
        _TME_SPARC_FPU_UNIMPL;
      }

      /* get %icc or %xcc, depending on opf bit seven: */
      cc = ic->tme_sparc64_ireg_ccr;
      if (opf & TME_BIT(7)) {
        cc /= (TME_SPARC64_CCR_XCC / TME_SPARC64_CCR_ICC);
      }
      cc = TME_FIELD_MASK_EXTRACTU(cc, TME_SPARC64_CCR_ICC);

      /* get the conditions mask: */
      conds_mask = _tme_sparc_conds_icc[cc];
    }

    /* otherwise, this uses floating-point condition codes: */
    else {

      /* get the right %fcc: */
      cc_i = TME_FIELD_MASK_EXTRACTU(opf, (0x3 << 6));
      if (cc_i == 0) {
        cc = TME_FIELD_MASK_EXTRACTU(ic->tme_sparc_fpu_fsr, TME_SPARC_FSR_FCC);
      }
      else {
        cc = (ic->tme_sparc_fpu_xfsr >> (2 * (cc_i - 1))) & 0x3;
      }

      /* get the conditions mask: */
      conds_mask = _tme_sparc_conds_fcc[cc];
    }

    /* add the not-conditions to the conditions mask: */
    conds_mask += ((~conds_mask) << 8);

    /* get the cond field: */
    cond = TME_BIT(TME_FIELD_MASK_EXTRACTU(TME_SPARC_INSN, (0xf << 14)));

    /* if the condition is not true: */
    if (!(conds_mask & cond)) {

      /* return now: */
      /* NB that this may expose us to guests, since we do not check
         that the floating-point register numbers are valid: */
      return;
    }

    /* clear bits six, seven, and eight in opf: */
    opf &= 0x3f;
  }

  /* dispatch on the opf field: */
  switch (opf) {
#define _TME_SPARC_FPU_FORMAT_RS1(format) fpreg_rs1 = tme_sparc_fpu_fpreg_read(ic, TME_SPARC_FORMAT3_MASK_RS1, (format))
#define _TME_SPARC_FPU_FORMAT_RS2(format) fpreg_rs2 = tme_sparc_fpu_fpreg_read(ic, TME_SPARC_FORMAT3_MASK_RS2, (format))
#define _TME_SPARC_FPU_FORMAT_RD(format) do { fpreg_rd_format = (format) | TME_IEEE754_FPREG_FORMAT_BUILTIN; fpreg_rd_number = tme_sparc_fpu_fpreg_decode(ic, fpreg_rd_number_encoded, fpreg_rd_format); } while (/* CONSTCOND */ 0)

  case 1:  /* 000000001 FMOVscc: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_SINGLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd = *fpreg_rs2;
    break;

  case 2:  /* 000000010 FMOVdcc: */
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RD(TME_IEEE754_FPREG_FORMAT_DOUBLE);
    _TME_SPARC_FPU_BEGIN;
    fpreg_rd = *fpreg_rs2;
    break;

  case 81:  /* 001010001 FCMPs: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_single_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    _tme_sparc_fpu_fcc_single(ic, &fpreg_rd, FALSE);
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 82:  /* 001010010 FCMPd: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_double_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    _tme_sparc_fpu_fcc_double(ic, &fpreg_rd, FALSE);
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 83:  /* 001010011 FCMPq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_quad_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    _tme_sparc_fpu_fcc_quad(ic, &fpreg_rd, FALSE);
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 85:  /* 001010101 FCMPEs: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_SINGLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_single_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    _tme_sparc_fpu_fcc_single(ic, &fpreg_rd, TRUE);
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 86:  /* 001010110 FCMPEd: */
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_DOUBLE | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_double_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    _tme_sparc_fpu_fcc_double(ic, &fpreg_rd, TRUE);
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  case 87:  /* 001010111 FCMPEq: */
    _TME_SPARC_FPU_UNIMPL_IF(TME_SPARC_FPU_FLAG_NO_QUAD);
    _TME_SPARC_FPU_FORMAT_RS1(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_FORMAT_RS2(TME_IEEE754_FPREG_FORMAT_QUAD | TME_IEEE754_FPREG_FORMAT_BUILTIN);
    _TME_SPARC_FPU_OP_DYADIC(tme_ieee754_ops_quad_sub,
                             fpreg_rs1, fpreg_rs2, &fpreg_rd);
    _tme_sparc_fpu_fcc_quad(ic, &fpreg_rd, TRUE);
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

  default:
    _TME_SPARC_FPU_UNIMPL;
    fpreg_rd_format = TME_IEEE754_FPREG_FORMAT_NULL;
    break;

#undef _TME_SPARC_FPU_FORMAT_RS1
#undef _TME_SPARC_FPU_FORMAT_RS2
#undef _TME_SPARC_FPU_FORMAT_RD
  }

  /* store any destination: */
  if (fpreg_rd_format != TME_IEEE754_FPREG_FORMAT_NULL) {
    tme_sparc_fpu_fpreg_format(ic, fpreg_rd_number, fpreg_rd_format);
    ic->tme_sparc_fpu_fpregs[fpreg_rd_number] = fpreg_rd;
    TME_SPARC_FPU_DIRTY(ic, fpreg_rd_number);
  }

}
