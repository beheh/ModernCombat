/*-- PIN Eingabe --*/

//Raum muss zum Bedienen der Konsole leer sein.

#strict 2
#appendto PKEY


protected func ControlThrow(object pClonk)
{
  if(FindObject2(Find_Func("IsClonk"), Find_InRect(AbsX(1060), AbsY(640), 110, 59)))
  {
      //Effekte
      Sound("Error");
      CreateParticle("NoGravSpark", 5, 1, 0, 0, 50, RGBa(255, 0, 0, 50));
      return(false);
  }

  clonk = pClonk;
  Menu(0);
}

private func CheckInput()
{
  if(input == number && !FindObject2(Find_Func("IsClonk"), Find_InRect(AbsX(1060), AbsY(640), 110, 59)))
  {
    if(target)
    {
      target->~Access();

      //Effekte
      Sound("BipBipBip");
      CreateParticle("NoGravSpark", 5, 1, 0, 0, 50, RGBa(0, 255, 0, 50));
      return(true);
    }
  }
  else
  {
      //Effekte
      Sound("Error");
      CreateParticle("NoGravSpark", 5, 1, 0, 0, 50, RGBa(255, 0, 0, 50));
      return(false);
  }
}