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

public class CostFunctionDelegate : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal CostFunctionDelegate(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(CostFunctionDelegate obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~CostFunctionDelegate() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NQuantLibcPINVOKE.delete_CostFunctionDelegate(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public virtual double value(QlArray x) {
    double ret = (SwigDerivedClassHasMethod("value", swigMethodTypes0) ? NQuantLibcPINVOKE.CostFunctionDelegate_valueSwigExplicitCostFunctionDelegate(swigCPtr, QlArray.getCPtr(x)) : NQuantLibcPINVOKE.CostFunctionDelegate_value(swigCPtr, QlArray.getCPtr(x)));
    if (NQuantLibcPINVOKE.SWIGPendingException.Pending) throw NQuantLibcPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual QlArray values(QlArray x) {
    QlArray ret = new QlArray((SwigDerivedClassHasMethod("values", swigMethodTypes1) ? NQuantLibcPINVOKE.CostFunctionDelegate_valuesSwigExplicitCostFunctionDelegate(swigCPtr, QlArray.getCPtr(x)) : NQuantLibcPINVOKE.CostFunctionDelegate_values(swigCPtr, QlArray.getCPtr(x))), true);
    if (NQuantLibcPINVOKE.SWIGPendingException.Pending) throw NQuantLibcPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public CostFunctionDelegate() : this(NQuantLibcPINVOKE.new_CostFunctionDelegate(), true) {
    if (NQuantLibcPINVOKE.SWIGPendingException.Pending) throw NQuantLibcPINVOKE.SWIGPendingException.Retrieve();
    SwigDirectorConnect();
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("value", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateCostFunctionDelegate_0(SwigDirectorvalue);
    if (SwigDerivedClassHasMethod("values", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateCostFunctionDelegate_1(SwigDirectorvalues);
    NQuantLibcPINVOKE.CostFunctionDelegate_director_connect(swigCPtr, swigDelegate0, swigDelegate1);
  }

  private bool SwigDerivedClassHasMethod(string methodName, global::System.Type[] methodTypes) {
    global::System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, global::System.Reflection.BindingFlags.Public | global::System.Reflection.BindingFlags.NonPublic | global::System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(CostFunctionDelegate));
    return hasDerivedMethod;
  }

  private double SwigDirectorvalue(global::System.IntPtr x) {
    return value(new QlArray(x, false));
  }

  private global::System.IntPtr SwigDirectorvalues(global::System.IntPtr x) {
    return QlArray.getCPtr(values(new QlArray(x, false))).Handle;
  }

  public delegate double SwigDelegateCostFunctionDelegate_0(global::System.IntPtr x);
  public delegate global::System.IntPtr SwigDelegateCostFunctionDelegate_1(global::System.IntPtr x);

  private SwigDelegateCostFunctionDelegate_0 swigDelegate0;
  private SwigDelegateCostFunctionDelegate_1 swigDelegate1;

  private static global::System.Type[] swigMethodTypes0 = new global::System.Type[] { typeof(QlArray) };
  private static global::System.Type[] swigMethodTypes1 = new global::System.Type[] { typeof(QlArray) };
}

}
