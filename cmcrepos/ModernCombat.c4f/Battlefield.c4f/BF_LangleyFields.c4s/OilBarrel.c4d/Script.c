/*-- �lfass --*/

#strict

#include BARL
#include XBRL

// Fassdaten
public func BarrelMaterial() { return(Material("Oil")); }
public func BarrelMaterialName() { return("Oil"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (iMaterial == Material("Oil")) return(OBRL);
  return(_inherited(iMaterial));
}

protected func Damage()
{
  // Verz�gert zerst�ren um Endlosrekursion zu verhindern
  Schedule(Format("Object(%d)->~Damaged()",ObjectNumber(this)),1);
  return(1);
}  

private func Damaged()
{
  CreateContents(BARL);
  Exit(Contents(), 0, 0, Random(360), Random(7) - 3, Random(3) - 8, +10);
  Explode(Max(GetComDir() / 6, 15));
}

/* F�r's �lf�rderungsziel */

public func Sale(plr) 
{
  var obj;
  if (obj = FindObject (OILP, 50 - GetX (), 49 - GetY ()))
    obj -> OILP::PlayerHasSold (plr, this ());
}

public func Purchase(int plr, object buyobj) 
{
  var obj;
  if (obj = FindObject (OILP, 50 - GetX (), 49 - GetY ()))
    obj -> OILP::PlayerHasBought (plr, this ());
}
