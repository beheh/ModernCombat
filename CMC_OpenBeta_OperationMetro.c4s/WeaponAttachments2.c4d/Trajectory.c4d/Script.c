/*-- Projektilvorschau --*/

#strict 2

static const g_CrosshairID = TRTY;

protected func Initialize()
{
  SetVisibility(VIS_Owner);
}

global func RemoveTrajectory(object pObj)
{
  // Finden und vernichten
  var pTrajectory = FindObject2(Find_ID(TRTY), Find_ActionTarget(pObj));
  if(pTrajectory) RemoveObject(pTrajectory);
}

global func AddTrajectory(object pObj, int iX, int iY, int iXDir, int iYDir, int iRange, int iColor)
{
  // Alte Vorschau löschen
  RemoveTrajectory(pObj);
  // Neues Hilfsobjekt erzeugen
  var pTrajectory = CreateObject(TRTY, GetX(pObj)-GetX(), GetY(pObj)-GetY(), GetOwner(pObj));
  pTrajectory->SetAction("Attach", pObj);
  // Startwerte setzen
  var i = -1, iXOld, iYOld;
  var iFaktor = 100;
  iX *= iFaktor; iY *= iFaktor;
  iYDir *= 5; iXDir *= 5;
  iY -= 4*iFaktor;
  iXOld = iX; iYOld = iY;
  iRange *= 2;
  // Flugbahn simulieren
  while(++i < iRange)
  {
    // Geschwindigkeit und Gravitation aufrechnen
    iX += iXDir;
    iY += iYDir + GetGravity()*i/20;
    // Wenn wir weit genug weg sind für einen neuen Punkt diesen einfügen
    if(Distance((iXOld-iX)/iFaktor, (iYOld-iY)/iFaktor)>=10)
    {
      CreateParticle("Aimer", iX/iFaktor-GetX(pTrajectory), iY/iFaktor-GetY(pTrajectory),
		      iXDir/500, iYDir/500, 10, iColor, pTrajectory);
      iXOld = iX; iYOld = iY;
    }
    // Oder ist es hier schon aus?
    if(GBackSolid(iX/iFaktor-GetX(), iY/iFaktor-GetY())) break;
  }
  // So, fertig
  return pTrajectory;
}

public func AttachTargetLost()
{
  RemoveObject();
}

public func RejectEntrance(object pIntoObj)
{
  return true;
}
