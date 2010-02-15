/*-- Ereignis-Anzeiger --*/

#strict

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
    return(msg);
  }
  else
  {
    msg = CreateObject(EI4K,0,0,iPlrPlusOne-1);
    msg->Set(...);
    return(msg);
  }
}

protected func Initialize()
{
  SetVisibility(VIS_Owner);
  Local(0) = 0;
  Local(1) = 0;
  SetColorDw(RGB(255,255,255));
}

local text,icon,color,graphics;

public func Set(string szText, id idIcon, int dwTextColor, int dwIconColor, string szGraphics)
{
  if(!idIcon)
  {
    idIcon = GetID();
    if(!szGraphics)
      szGraphics = "Default";
  }

  if(!dwTextColor)
    dwTextColor = RGB(255,255,255);
    
  if(!dwIconColor)
    dwIconColor = RGB(255,255,255);
    
  text = szText;
  icon = idIcon;
  color = dwTextColor;
  graphics = szGraphics;
  SetColorDw(dwIconColor);
  
  Sound("EI4K_NewMessage.ogg",true,0,0,GetOwner()+1);
  
  AddEffect("IntEventInfo",this(),10,1,this(),EI4K);
}

public func FxIntEventInfoStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return();

  SortByActTime();
  SetGraphics(graphics,0,icon,1,GFXOV_MODE_IngamePicture);
  FxIntEventInfoTimer(pTarget,iEffectNumber,0);
}

public func FxIntEventInfoTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var a = 255-Sin(900+(iEffectTime*900/MaxTime()),255*2,10)/2;
  
  SetClrModulation(RGBa(255,255,255,a),this(),1); 
  CustomMessage(Format("           %s",text), pTarget, GetOwner(), 0, 26, SetRGBaValue(color,a), 0, 0, MSG_NoLinebreak);
  if(iEffectTime >= MaxTime()) return(-1);
}

public func FxIntEventInfoStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(!fTemp)
    RemoveObject(pTarget);
}

private func Destruction()//*hack*
{
  SortByActTime(this());
}

private func SortByActTime(object pExclude)
{
  var aSymbols = FindObjects(Find_Owner(GetOwner()),Find_ID(GetID()),Find_Exclude(pExclude));
  var temp;
  
  if(!pExclude)//*cheat*
    aSymbols[GetLength(aSymbols)] = this();

  //Sortieren.
  while(!Sorted(aSymbols))
  {
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
  }

  //Überschüssige Einträge löschen.
  for(var i = 0;i < GetLength(aSymbols);i++)
  {
    SetPosition(IconSize(),80+i*IconSize(),aSymbols[i]);

    if(i > MaxInfos())
      RemoveEffect("IntEventInfo",aSymbols[i]);
  }
}

private func Sorted(&aSymbols)
{
  if(GetLength(aSymbols) == 1)
    return(true);

  for(var i = 0;i < GetLength(aSymbols);i++)
  {
    if(i == 0)
      continue;

    if(aSymbols[i]->EI4K::GetEffectTime() < aSymbols[i-1]->EI4K::GetEffectTime())
    {
      return(false);
    }
  }
  return(true);
}

public func GetEffectTime()
{
  return(GetEffect("IntEventInfo",this(),0,6));
}

/* Interne Konstanten und Tags */

public func MaxInfos(){return(12);}
public func MaxTime(){return(35*12);}
public func IconSize(){return(23);}
public func IsHUD(){return(1);}