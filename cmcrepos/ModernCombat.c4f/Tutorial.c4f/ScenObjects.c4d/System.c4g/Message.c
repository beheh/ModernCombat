/*-- Message --*/

#strict

//kleine Helper-Funktion
global func DoInfoMessage(object pTarget, string szName, string szText, id idPortrait, int dwClrMod, int iNum)
{
  //Ziel absichern
  if(!pTarget) pTarget = this();
  if(!pTarget) return();
  
  //Portrait
  if(!idPortrait) idPortrait = CLNK;
  if(!iNum) iNum = 1;
  var portrait = Format("Portrait:%i::%x::%d",idPortrait,dwClrMod,iNum);
  
  //Menü und so
  CreateMenu(NONE, pTarget, pTarget, 0, "", 0, 3, false);
  AddMenuItem(portrait, "", NONE, pTarget, 0, 0, "", 5, 0, 0);
  AddMenuItem(Format("<%s>|%s",szName,szText), "", NONE, pTarget, 0, 0, "", 512, 0, 0);
  SetMenuTextProgress(GetLength(szName)+1, pTarget);
  
  //OK-Knopf
  AddMenuItem("OK", "", TALK, pTarget, 0, 0, "", 512);
}