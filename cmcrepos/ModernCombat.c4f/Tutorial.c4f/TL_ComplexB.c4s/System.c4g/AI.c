/* Henrys KI-Sandkasten */
#strict

//Plaziert einen KI-Clonk.
global func PlaceAI(int iX, int iY, string szType, id idType)
{
  if(!idType) idType = CLNK;
  if(!szType) return();

  var ai = CreateObject(idType,iX,iY,NO_OWNER);
  ai->StartAI(szType);
  ai->SetColorDw(HSL(Random(256),255,127));
  return(ai);
}

//Startet die KI für einen Clonk.
global func StartAI(string szBehaviour, object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  return(AddEffect ("IntAIEffect", pClonk, 10, 10, 0, 0, szBehaviour));
}

//Stoppt eine Clonk-KI.
global func StopAI(object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  while(GetEffect("IntAIEffect",pClonk))
    RemoveEffect("IntAIEffect", pClonk);
}

//Gibt die Referenz auf eine KI-Variable zurück.
global func & AIVar(int iIndex, object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  
  var effect = GetEffect("IntAIEffect",pClonk);
  return(EffectVar(iIndex+4, pClonk, effect));
}

//Gibt den KI-Effekt eines Clonk zurück.
global func GetAI(object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  
  return(GetEffect("IntAIEffect",pClonk));
}

//Gibt den KI-Typ (String) zurück.
global func GetAIType(object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  
  var effect = GetEffect("IntAIEffect",pClonk);
  if(effect)
    return(EffectVar(0,pClonk,effect));
}

//Gibt den Dialoghelfer der KI zurück.
global func AIDialog(object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  
  var effect = GetEffect("IntAIEffect",pClonk);
  if(effect)
    return(EffectVar(1,pClonk,effect));
}

global func AIHomeX(object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  
  var effect = GetEffect("IntAIEffect",pClonk);
  if(effect)
    return(EffectVar(2,pClonk,effect));
}

global func AIHomeY(object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  
  var effect = GetEffect("IntAIEffect",pClonk);
  if(effect)
    return(EffectVar(3,pClonk,effect));
}

global func AIGoHome(object pClonk)
{
  if(!pClonk) pClonk = this();
  if(!pClonk) return();
  
  var effect = GetEffect("IntAIEffect",pClonk);
  if(effect)
  {
    var x = EffectVar(2,pClonk,effect);
    var y = EffectVar(3,pClonk,effect);
    SetCommand(pClonk,"MoveTo",0,x,y);
  }
}

/* Helper-Effekt */
global func FxIntAIEffectStart(object pTarget, int iEffectNumber, int iTemp,szBehaviour)
{
  EffectVar(0, pTarget, iEffectNumber) = szBehaviour;
  var talker = CreateObject(TALK,0,0,NO_OWNER);
  talker->SetTarget(pTarget);
  EffectVar(1, pTarget, iEffectNumber) = talker;
  
  EffectVar(2, pTarget, iEffectNumber) = GetX(pTarget);
  EffectVar(3, pTarget, iEffectNumber) = GetY(pTarget);
  
  GameCall(Format("AI_%s_Init",szBehaviour),pTarget,iEffectNumber);//AI_<szBehaviour>_Init(object pClonk, int iEffect){}
  
  return(1);
}

global func FxIntAIEffectTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  GameCall(Format("AI_%s_Timer",EffectVar(0, pTarget, iEffectNumber)),pTarget, iEffectNumber);//AI_<szBehaviour>_Timer(object pClonk, int iEffect){}
  return(0);
}