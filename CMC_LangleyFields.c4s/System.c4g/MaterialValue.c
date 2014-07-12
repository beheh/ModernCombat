/*-- Materialpreise --*/

#strict 2

//Legt die Preise für 1px Materialien fest (in Promille)
global func GetMaterialValue(int iMat)
{
	if(iMat == Material("Oil")) return 500; //0.5 Credits/px
	if(iMat == Material("Water")) return 100; //0.1 Credits/px
  
  return 0; 
}
