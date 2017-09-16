/*-- Punkteanzeige --*/

#strict 2
#include EI4K

public func MaxInfos()	{return 6;}	//Maximale Anzahl gleichzeitig eingeblendeter Eventinfos
public func MaxTime()	{return 35*8;}	//Zeit bis zum Ausblenden


/* Initialisierung */

protected func Initialize()
{
  if(GetOwner() != NO_OWNER)
    SetVisibility(VIS_Owner);
  else
    RemoveObject();
  SetColorDw(RGB(255,255,255));
}

/* Globaler Aufruf */

global func ScoreInfo(int iPlrPlusOne)
{
  var msg;

  //Nachricht muss Besitzer haben
  if(iPlrPlusOne <= 0)
    return;
  else
  {
    msg = CreateObject(SI4K,0,0,iPlrPlusOne-1);
    msg->Set(...);
    return msg;
  }
}

public func Set(string szText, id idIcon, int dwTextColor, int dwIconColor, string szGraphics, string szSound, string szAction)
{
  //Falls nicht mitgegeben, Standardwerte setzen
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

  //Sound für betreffenden Spieler abspielen
  if (GetOwner() != NO_OWNER)
  {
    if(!szSound) szSound = "RWDS_Points*.ogg";
    Sound(szSound,true,0,0,GetOwner()+1);
  }

  AddEffect("IntScoreInfo", this, 10, 1, this, SI4K);
}

public func FxIntScoreInfoStart(object pTarget, int iEffectNumber, int iTemp)
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
  FxIntScoreInfoTimer(pTarget, iEffectNumber, 0);
}

public func FxIntScoreInfoTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nachricht bei totem Spieler entfernen
  if (!fGlobal && !GetPlayerName(GetOwner(pTarget)))
    return -1;
  var a = 255-Sin(900+(iEffectTime*900/MaxTime()),255*2,10)/2;

  if(icon) SetClrModulation(RGBa(255,255,255,a),this,1); 
  CustomMessage(Format("            %s",text), pTarget, GetOwner(), 0, 26, SetRGBaValue(color,a), 0, 0, MSG_NoLinebreak);
  if(iEffectTime >= MaxTime()) return -1;
}

public func FxIntScoreInfoStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(!fTemp)
    RemoveObject(pTarget);
}

private func SortByActTime(object pExclude)
{
  var aSymbols = FindObjects(Find_Owner(GetOwner()), Find_ID(GetID()), Sort_Reverse());
  if(pExclude)
    aSymbols = FindObjects(Find_Owner(GetOwner()), Find_ID(GetID()), Find_Exclude(pExclude), Sort_Reverse());

  var temp;

  //Einträge sortieren
  while(!Sorted(aSymbols))
    for(var i = 0;i < GetLength(aSymbols);i++)
    {
      if(i == 0)
        continue;

      if(aSymbols[i]->SI4K::GetEffectTime() < aSymbols[i-1]->SI4K::GetEffectTime())
      {
        temp = aSymbols[i];
        aSymbols[i] = aSymbols[i-1];
        aSymbols[i-1] = temp;
      }
    }

  //Überschüssige Einträge entfernen
  for(var i = 0;i < GetLength(aSymbols);i++)
  {
    SetPosition(IconSize()+20,-138-i*IconSize(),aSymbols[i]);

    if(i > MaxInfos())
      RemoveEffect("IntScoreInfo",aSymbols[i]);
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

    if(aSymbols[i]->SI4K::GetEffectTime() < aSymbols[i-1]->SI4K::GetEffectTime())
      return false;
  }
  return true;
}

public func GetEffectTime()
{
  return GetEffect("IntScoreInfo",this,0,6);
}