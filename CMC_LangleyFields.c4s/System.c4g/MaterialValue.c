/*-- Materialpreise --*/

#strict 2


//Legt die Preise für 1px Materialien fest (in Promille)
global func GetMaterialValue(int iMat)
{
  if(iMat == Material("Oil")) return 125;  //0.125 Credits/px
  if(iMat == Material("Water")) return 40; //0.04  Credits/px
  if(iMat == Material("Acid")) return 50;  //0.05  Credits/px
  if(iMat == Material("Lava")) return 50;  //0.05  Credits/px

  return 0; 
}