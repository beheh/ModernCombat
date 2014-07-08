/*-- Quelle --*/

#strict 2

local amount, drain, iMaterial, iX, iY;


/* Initialisierung */

protected func Initialize()
{
  SetAction("Drain");
  amount = 2;
  iMaterial = 1;
  iX = 0;
  iY = 0;
  return 1;
}

private func Draining()
{
  if(GBackSolid()) SetPosition(GetX(), GetY()-1);	
  var i;
  var mat;

  //Material setzen
  if(iMaterial==1) mat = Material("Water");
  if(iMaterial==2) mat = Material("Acid");
  if(iMaterial==3) mat = Material("DuroLava");
  if(iMaterial==4) mat = Material("Oil");
  //Erzeugen sofern zutreffend
  if(!drain)
  {
    i=amount;
    while(i--) InsertMaterial(mat,0,0,iX, iY);
    return;
  }
  i = drain->ExtractMaterialAmount(0, 0, drain->GetMaterial(), amount);
  Message("%d %s", this(), i, MaterialName(mat));
  while(i--) InsertMaterial(mat);
  return 1;
}

/* Verhalten festlegen */

func Set(iamount, iMat, iXDir, iYDir)
{
  amount = iamount;
  iMaterial = iMat;
  iX = iXDir;
  iY = iYDir;

  return 1;
}