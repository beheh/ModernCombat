/*-- Flugbahnanzeige --*/

#strict 2

static const g_CrosshairID = TRTY;


/* Initialisierung */

protected func Initialize()
{
  SetVisibility(VIS_Owner);
}

/* Flugbahn erstellen */

global func AddTrajectory(object pObj, int iX, int iY, int iXDir, int iYDir, int iRange, int iColor)
{
  //Alte Flugbahn entfernen falls vorhanden
  RemoveTrajectory(pObj);
  //Neues Flugbahnobjekt erzeugen
  var pTrajectory = CreateObject(TRTY, GetX(pObj)-GetX(), GetY(pObj)-GetY(), GetOwner(pObj));
  pTrajectory->SetAction("Attach", pObj);

  //Startwerte setzen
  var i = -1, iXOld, iYOld;
  var iFaktor = 100;
  iX *= iFaktor; iY *= iFaktor;
  iYDir *= 5; iXDir *= 5;
  iY -= 4*iFaktor;
  iXOld = iX; iYOld = iY;
  iRange *= 2;
  //Flugbahn simulieren
  while(++i < iRange)
  {
    //Geschwindigkeits- und Gravitationseinfluss miteinbeziehen
    iX += iXDir;
    iY += iYDir + GetGravity()*i/20;
    //Weitere Punkte einfügen
    if(Distance((iXOld-iX)/iFaktor, (iYOld-iY)/iFaktor)>=10)
    {
      CreateParticle("Aimer", iX/iFaktor-GetX(pTrajectory), iY/iFaktor-GetY(pTrajectory),
      				iXDir/500, iYDir/500, 10, iColor, pTrajectory);
      iXOld = iX; iYOld = iY;
    }
    if(GBackSolid(iX/iFaktor-GetX(), iY/iFaktor-GetY())) break;
  }
  return pTrajectory;
}

/* Entfernung */

global func RemoveTrajectory(object pObj)
{
  var pTrajectory = FindObject2(Find_ID(TRTY), Find_ActionTarget(pObj));
  if(pTrajectory) RemoveObject(pTrajectory);
}

public func AttachTargetLost()
{
  RemoveObject();
}

public func RejectEntrance(object pIntoObj)
{
  return true;
}