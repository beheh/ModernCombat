/*-- Schalter --*/

#strict

// Zielobjekte
local target;

public func Set(object t) { target = t; }

/* Steuerung */

protected func ControlThrow()
{
  if(target) target->~Access();
}

public func ActivateEntrance(pObj)
{
  FinishCommand (pObj,true,0); 
  ControlThrow();
  return(1);
}


// Mehrere Ziele...
//
// Ziel hinzufügen: pTarget = Zielobjekt, CallFunc = Funktion, die aufgerufen werden soll (0 für "Switch")
//public func AddTarget(object pTarget, string CallFunc)
//{
//  targets[GetLength(targets)] = CreateArray();
//  targets[GetLength(targets)-1][0] = pTarget;
//  targets[GetLength(targets)-1][1] = CallFunc;
//  return(1);
//}
//
// Ziel entfernen
//public func RemoveTarget(object pTarget)
//{
//  var i=-1;
//  for(var step in targets)
//  {
//    i++;
//    if(!step) continue;
//    if(step[0] == pTarget)
//    {
//      targets[i] = 0;
//      return(1);
//    }
//  }
//}
