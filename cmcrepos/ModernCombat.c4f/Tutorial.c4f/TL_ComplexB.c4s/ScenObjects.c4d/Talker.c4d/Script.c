/*-- Talker --*/

#strict
#include _DIA

local talkto, greet;

func Initialized()
{
  SetVisibility(VIS_None);
}

/* Kontrolle */
public func SetTarget(object target)
{
  SetAction("Attach",target);
  SetName(GetName(target));
  talkto = target;
}

public func SetGreeting(string szGreeting)
{
  greet = szGreeting;
}

public func GetPartner()
{
  return(pPartner);
}

public func AddAnswer(object pTarget, string szText, string szFunc, id idItem, vParam, object pCallTarget, int iLink)
{
  // Parameter
  if(!pTarget)
    if(!(pTarget = pPartner))
      return();
      
  if(!szText)
    return();
    
  var param = NewAnswerInfo(szFunc,vParam,pCallTarget,iLink,"AInfo2");
  
  if(AddMenuItem(szText, "EndChat", idItem, pTarget, 0, param, "", 512))
    return(true);
}

public func AddAnswer2(object pTarget, int iAInfo)
{
  // Parameter
  if(!pTarget)
    if(!(pTarget = pPartner))
      return();
      
  var szText = EffectVar(4, this(),  iAInfo);
      
  if(!szText)
    return();
  
  if(AddMenuItem(szText, "EndChat", NONE, pTarget, 0, iAInfo, "", 512))
    return(true);
}

/* Diverses */
public func ChatFinished(i)
{
  if(GetAI(talkto))
    GameCall(Format("AI_%s_StopDialog",GetAIType(talkto)),talkto,GetAI(talkto),pPartner);//AI_<szBehaviour>_StopDialog(object pClonk, int iEffect, object pPartner){}
  else
    GameCall("ChatFinished",talkto,i,this(),pPartner);
}

public func ActivateEntrance(object pBy)
{
  if(GetAI(talkto))
    GameCall(Format("AI_%s_StartDialog",GetAIType(GetActionTarget())),talkto,GetAI(talkto),pBy);//AI_<szBehaviour>_StartDialog(object pClonk, int iEffect, object pPartner){}
  else
    StartDialog(pBy);
}

public func StartDialog(object pBy)
{
  //Stehen bleiben! >:0
  SetComDir(COMD_None, pBy);
  SetCommand(pBy, "None");
  if(!(GetOCF(pBy) & OCF_CrewMember)) return();
  
  if(pPartner) return();
  // Gespräch mit Spieler
  pPartner = pBy;
  // Menü
  CloseMenu (pPartner);//erstmal zu tun
  if(!CreateMenu(GetID(), pPartner, this(), 0, "", 0, 3, false)) return();
  // Einträge
  var szPortrait = GetMenuPortrait(this());
  // Portrait
  AddMenuItem(szPortrait, "", NONE, pPartner, 0, 0, "", 5, 0, 0);
  // Begrüßungsgelaber
  var greeting = Format("<%s>|%s", GetName(this()), Greeting());
  AddMenuItem(greeting, "", NONE, pPartner, 0, 0, "", 512, 0, 0);
	//SetMenuDecoration();
	SetMenuTextProgress(GetLength(GetName())+1, pPartner);
}

public func Greeting() { return(greet); }

public func GetMenuPortrait()
{
  return(Format("Portrait:%i::%x::%s", GetID(talkto), GetColorDw(talkto), "1"));
}

public func EndChat(id dummy, int iEffect)
{
  ChatFinished(iEffect);
  greet=0;
  var p = pPartner;
  pPartner = 0;

  var fx     = EffectVar(0, this(),  iEffect);
  var param  = EffectVar(1, this(),  iEffect);
  var target = EffectVar(2, this(),  iEffect);
  var link   = EffectVar(3, this(),  iEffect);
  
  UpdateDisabled(iEffect,p,link);
  
  if(fx)
  {
    if(target)
      target->~Call(fx,param);
    else
      GameCall(fx,param,talkto,p);
  }
   
  if(link)
    if(!EffectVar(5,this(),iEffect)[GetPlayerID(GetController(p))])
      StartChat(link,p);
    
  //ClearAnswerInfo();//*lösch*
}

private func UpdateDisabled(int iEffect, object pClonk, int iLink)
{
  if(iLink)
  {
    if(CheckDisabled(iLink,GetController(pClonk)))
    {
      EffectVar(5,this(),iEffect)[GetPlayerID(GetController(pClonk))] = true;
    }
  }
  else
  {
    EffectVar(5,this(),iEffect)[GetPlayerID(GetController(pClonk))] = true;
  }
}

private func CheckDisabled(int iIndex, int iPlayerID, int iDepth)
{
  var iEffect = ChatIndex2Effect(iIndex);

  var aAInfo = EffectVar(1, this(), iEffect);
  
  for(var info in aAInfo)
  {
    //var link = EffectVar(3,this(),info);
    
    /*if(iDepth <= 6)
      if(link)
        if(!CheckDisabled(link,iPlayerID,iDepth))
          return(false);*/
          
    if(!EffectVar(5,this(),info)[iPlayerID])
      return(false);
  }
  
  return(true);
}

/* Infos */
public func NewAnswerInfo(string szFunc, vParam, object pTarget, int iLink, string szAnswer, string szAInfoType)//Wow. Strukturen in Clonk?! O.o
{
  if(!szAInfoType)
    szAInfoType = "AInfo";
  szAInfoType = Format("Int%s",szAInfoType);

  var effect = AddEffect (szAInfoType, this(), 10, 0, this(), GetID());
  
  EffectVar(0, this(), effect) = szFunc;//Funktion
  EffectVar(1, this(), effect) = vParam;//Parameter
  EffectVar(2, this(), effect) = pTarget;//Zielobjekt.... 0 = SzenScript :D
  EffectVar(3, this(), effect) = iLink;//Chat-Link für Chat-Strukturen - NextGen-Tech!
  EffectVar(4, this(), effect) = szAnswer;//Antwort... wird meistens nicht gebraucht.
  EffectVar(5, this(), effect) = [];//indizierter Spieler-Array
  
  return(effect);
}

public func ClearAnswerInfo(string szAInfoType)
{
  if(!szAInfoType)
    szAInfoType = "AInfo2";
  szAInfoType = Format("Int%s",szAInfoType);
  
  while(GetEffect(szAInfoType,this()))
    RemoveEffect(szAInfoType,this());
}


/* t0lle Chat-Struktur */
public func NewChat(string szGreeting, array aAInfo)
{
  var effect = AddEffect("IntChat", this(), 10, 0, this(), GetID());
  
  EffectVar(0, this(), effect) = szGreeting;
  EffectVar(1, this(), effect) = aAInfo;
  return(effect);
}

public func TestChat(int iIndex, object pClonk)
{
  if(!iIndex) return();
  if(!pClonk) pClonk = pPartner;
  if(!pClonk) return();
  
  iIndex = GetEffectCount("IntChat",this()) - iIndex;
  var iEffect = GetEffect("IntChat", this(), iIndex);
  var aAInfo = EffectVar(1, this(), iEffect);
  
  for(var AInfo in aAInfo)
  {
    if(!EffectVar(5,this(),AInfo)[GetPlayerID(GetController(pClonk))])
    {
      return(true);
    }
  }
  
  return(false);
}

public func StartChat(int iIndex, object pClonk)
{
  SetComDir(COMD_None, pClonk);
  SetCommand(pClonk, "None");

  if(!iIndex) return();
  if(!pClonk) pClonk = pPartner;
  if(!pClonk) return();
  
  var iIndex2 = GetEffectCount("IntChat",this()) - iIndex;//Wegen lustigen mathematischen Glücksfällen kann ich das -1+1 weglassen. ^^
  var iEffect = GetEffect("IntChat", this(), iIndex2);
  var aAInfo = EffectVar(1, this(), iEffect);
  
  for(var AInfo in aAInfo)
  {
    if(!EffectVar(5,this(),AInfo)[GetPlayerID(GetController(pClonk))])
    {
      var link = EffectVar(3,this(),AInfo);
      if(link)
        if(CheckDisabled(link,GetPlayerID(GetController(pClonk))))
        {
          EffectVar(5,this(),AInfo)[GetPlayerID(GetController(pClonk))] = true;
        }
    }
  }
  
  if(!TestChat(iIndex,pClonk))
    return();
  
  SetGreeting(EffectVar(0, this(), iEffect));
  StartDialog(pClonk);
  
  for(var AInfo in aAInfo)
  {
    if(!EffectVar(5,this(),AInfo)[GetPlayerID(GetController(pClonk))])
    {
      AddAnswer2(pClonk,AInfo);
    }
  }
  
  return(1);
}

func ChatIndex2Effect(int iIndex)
{
  if(!iIndex) return(0);
  iIndex = GetEffectCount("IntChat",this()) - iIndex;
  var iEffect = GetEffect("IntChat",this(), iIndex);
  return(iEffect);
}

func StopChat()
{
  if(pPartner)
    pPartner->CloseMenu();
  greet=0;
  pPartner = 0;
}