//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.8
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace QuantLib {

public class BlackVarianceSurface : BlackVolTermStructure {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal BlackVarianceSurface(global::System.IntPtr cPtr, bool cMemoryOwn) : base(NQuantLibcPINVOKE.BlackVarianceSurface_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(BlackVarianceSurface obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~BlackVarianceSurface() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NQuantLibcPINVOKE.delete_BlackVarianceSurface(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public BlackVarianceSurface(Date referenceDate, Calendar cal, DateVector dates, DoubleVector strikes, Matrix blackVols, DayCounter dayCounter, _BlackVarianceSurface.Extrapolation lower, _BlackVarianceSurface.Extrapolation upper, string interpolator) : this(NQuantLibcPINVOKE.new_BlackVarianceSurface__SWIG_0(Date.getCPtr(referenceDate), Calendar.getCPtr(cal), DateVector.getCPtr(dates), DoubleVector.getCPtr(strikes), Matrix.getCPtr(blackVols), DayCounter.getCPtr(dayCounter), (int)lower, (int)upper, interpolator), true) {
    if (NQuantLibcPINVOKE.SWIGPendingException.Pending) throw NQuantLibcPINVOKE.SWIGPendingException.Retrieve();
  }

  public BlackVarianceSurface(Date referenceDate, Calendar cal, DateVector dates, DoubleVector strikes, Matrix blackVols, DayCounter dayCounter, _BlackVarianceSurface.Extrapolation lower, _BlackVarianceSurface.Extrapolation upper) : this(NQuantLibcPINVOKE.new_BlackVarianceSurface__SWIG_1(Date.getCPtr(referenceDate), Calendar.getCPtr(cal), DateVector.getCPtr(dates), DoubleVector.getCPtr(strikes), Matrix.getCPtr(blackVols), DayCounter.getCPtr(dayCounter), (int)lower, (int)upper), true) {
    if (NQuantLibcPINVOKE.SWIGPendingException.Pending) throw NQuantLibcPINVOKE.SWIGPendingException.Retrieve();
  }

  public BlackVarianceSurface(Date referenceDate, Calendar cal, DateVector dates, DoubleVector strikes, Matrix blackVols, DayCounter dayCounter, _BlackVarianceSurface.Extrapolation lower) : this(NQuantLibcPINVOKE.new_BlackVarianceSurface__SWIG_2(Date.getCPtr(referenceDate), Calendar.getCPtr(cal), DateVector.getCPtr(dates), DoubleVector.getCPtr(strikes), Matrix.getCPtr(blackVols), DayCounter.getCPtr(dayCounter), (int)lower), true) {
    if (NQuantLibcPINVOKE.SWIGPendingException.Pending) throw NQuantLibcPINVOKE.SWIGPendingException.Retrieve();
  }

  public BlackVarianceSurface(Date referenceDate, Calendar cal, DateVector dates, DoubleVector strikes, Matrix blackVols, DayCounter dayCounter) : this(NQuantLibcPINVOKE.new_BlackVarianceSurface__SWIG_3(Date.getCPtr(referenceDate), Calendar.getCPtr(cal), DateVector.getCPtr(dates), DoubleVector.getCPtr(strikes), Matrix.getCPtr(blackVols), DayCounter.getCPtr(dayCounter)), true) {
    if (NQuantLibcPINVOKE.SWIGPendingException.Pending) throw NQuantLibcPINVOKE.SWIGPendingException.Retrieve();
  }

  public static readonly _BlackVarianceSurface.Extrapolation ConstantExtrapolation = (_BlackVarianceSurface.Extrapolation)NQuantLibcPINVOKE.BlackVarianceSurface_ConstantExtrapolation_get();
  public static readonly _BlackVarianceSurface.Extrapolation InterpolatorDefaultExtrapolation = (_BlackVarianceSurface.Extrapolation)NQuantLibcPINVOKE.BlackVarianceSurface_InterpolatorDefaultExtrapolation_get();
}

}