/*-- Laser --*/

//Modifiziert den hazard-eigenen Lasereffekt für CMC.

#strict 2
#appendto LASR


public func Set(int iAngle, int iWidth, int iDist, int iT, object pDmg, object pAtt, int iReflect, bool fNoStuckRemove)
{
  //Optionale Parameter
  if(!iWidth) iWidth = 3;
  if(!iDist) iDist = 300;

  iTime = iT;
	reflections = iReflect;

  //Fade-Timer zurücksetzen
  c = 0;
  //Optionale Schadens- und Anhangsobjekte
  pDmgCallback = pDmg;
  pAttach = pAtt;
  if(pAttach)
  {
    dx = GetX()-GetX(pAttach);
    dy = GetY()-GetY(pAttach);
  }

  //Licht erstellen
  pLight = CreateObject(LALI, 0, 0, -1);

  SetMaxDistance(iDist);
  SetAngle(iAngle);
  SetWidth(iWidth);
  HitCheck();
  DrawTransform();

  //Bei feststecken entfernen
  if(Stuck() && !fNoStuckRemove)
    RemoveObject();
}