/*-- Ereignis-Anzeiger --*/

#strict 2

global func EventInfo4K(int iPlrPlusOne)
{
  var msg;
  
  if(iPlrPlusOne <= 0)
  {
    for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
    {
      msg = CreateObject(EI4K,0,0,GetPlayerByIndex(i,C4PT_User));
      msg->Set(...);
    }
    //Godmode. Nur wenns sowieso jeder sieht.
    msg = CreateObject(EI4K,0,0,-1);
	msg->SetAsGlobal();
    msg->Set(...);
    return msg;
  }
  else
  {
    msg = CreateObject(EI4K,0,0,iPlrPlusOne-1);
    msg->Set(...);
    return msg;
  }
}

protected func Initialize()
{
  if (GetOwner() != NO_OWNER)
    SetVisibility(VIS_Owner);
  else
    SetVisibility(VIS_God);
  SetColorDw(RGB(255,255,255));
}

local text, icon, color, graphics, action, fGlobal;

public func Set(string szText, id idIcon, int dwTextColor, int dwIconColor, string szGraphics, string szSound, string szAction)
{
  if(!idIcon && !szGraphics)
    szGraphics = "Default";

  if(!dwTextColor)
    dwTextColor = RGB(255,255,255);
    
  if(!dwIconColor)
    dwIconColor = RGB(255,255,255);
    
  text = szText;
  icon = idIcon;
  color = dwTextColor;
  graphics = szGraphics;
  action = szAction;
  SetColorDw(dwIconColor);

  if (GetOwner() != NO_OWNER) {
    if(!szSound) szSound = "EI4K_NewMessage.ogg";
    Sound(szSound,true,0,0,GetOwner()+1);
  }
  
  AddEffect("IntEventInfo", this, 10, 1, this, EI4K);
}

public func SetAsGlobal() { fGlobal = true; }

public func FxIntEventInfoStart(object pTarget, int iEffectNumber, int iTemp)
{
  SortByActTime();
  if (icon)
    if (action)
    {
      SetGraphics(graphics, pTarget, icon, 1, GFXOV_MODE_Action, action);
      var size = pTarget->~IconSize() * 1000 / Max(GetActMapVal("Facet", action, icon, 2), GetActMapVal("Facet", action, icon, 3));
      SetObjDrawTransform(size, 0, 0, 0, size, 0, pTarget, 1);
    }
    else
      SetGraphics(graphics, pTarget, icon, 1, GFXOV_MODE_IngamePicture);
  FxIntEventInfoTimer(pTarget, iEffectNumber, 0);
}

public func FxIntEventInfoTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Spieler tot, Nachricht weg.
  if (!fGlobal && !GetPlayerName(GetOwner(pTarget)))
    return -1;
  var a = 255-Sin(900+(iEffectTime*900/MaxTime()),255*2,10)/2;
  
  if(icon) SetClrModulation(RGBa(255,255,255,a),this,1); 
  CustomMessage(Format("           %s",text), pTarget, GetOwner(), 0, 26, SetRGBaValue(color,a), 0, 0, MSG_NoLinebreak);
  if(iEffectTime >= MaxTime()) return -1;
}

public func FxIntEventInfoStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(!fTemp)
    RemoveObject(pTarget);
}

private func Destruction()
{
  SortByActTime(this);
}

private func SortByActTime(object pExclude)
{
  var aSymbols = FindObjects(Find_Owner(GetOwner()), Find_ID(GetID()), Find_Exclude(pExclude), Sort_Reverse());
  var temp;
  
  if(!pExclude)
    aSymbols[GetLength(aSymbols)] = this;

  //Sortieren.
  while(!Sorted(aSymbols))
    for(var i = 0;i < GetLength(aSymbols);i++)
    {
      if(i == 0)
        continue;

      if(aSymbols[i]->EI4K::GetEffectTime() < aSymbols[i-1]->EI4K::GetEffectTime())
      {
        temp = aSymbols[i];
        aSymbols[i] = aSymbols[i-1];
        aSymbols[i-1] = temp;
      }
    }

  //�bersch�ssige Eintr�ge l�schen.
  for(var i = 0;i < GetLength(aSymbols);i++)
  {
    SetPosition(IconSize()+5,70+i*IconSize(),aSymbols[i]);

    if(i > MaxInfos())
      RemoveEffect("IntEventInfo",aSymbols[i]);
  }
}

private func Sorted(&aSymbols)
{
  if(GetLength(aSymbols) == 1)
    return true;

  for(var i = 0;i < GetLength(aSymbols);i++)
  {
    if(i == 0)
      continue;

    if(aSymbols[i]->EI4K::GetEffectTime() < aSymbols[i-1]->EI4K::GetEffectTime())
      return false;
  }
  return true;
}

public func GetEffectTime()
{
  return GetEffect("IntEventInfo",this,0,6);
}

/* Interne Konstanten und Tags */

public func MaxInfos(){return 12;}
public func MaxTime(){return 35*12;}
public func IconSize(){return 23;}
public func IsHUD(){return 1;}
