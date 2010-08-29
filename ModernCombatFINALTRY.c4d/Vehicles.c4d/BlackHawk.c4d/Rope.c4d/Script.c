/*-- Seil --*/

#strict 2

local iLength, iCalcedLength, iOCF, iVtx1, iVtx2;
local aPointsX, aPointsY;
local pPulley;
local fNoPickUp_0, fNoPickUp_1;

protected func Initialize()
{
  // OCF's, an die verbunden werden kann
  iOCF = OCF_Living | OCF_Grab | OCF_Chop | OCF_Collectible;

  aPointsX = [0,0];
  aPointsY = [0,0];
  return 1;
}

public func ConnectObjects(pObj1, pObj2)
{
	var p1 = pObj1->~GetRopeAttach(this);
	if(p1) pObj1 = p1;
	var p2 = pObj2->~GetRopeAttach(this);
	if(p2) pObj2 = p2;
  iVtx1 = GetMiddlestVertex(pObj1);
  iVtx2 = GetMiddlestVertex(pObj2);
  SetPoint(0, GetX(pObj1) + GetVertex (iVtx1, 0, pObj1), GetY(pObj1) + GetVertex(iVtx1, 1, pObj1));
  SetPoint(1, GetX(pObj2) + GetVertex (iVtx2, 0, pObj2), GetY(pObj2) + GetVertex(iVtx2, 1, pObj2));
  SetAction("Connect", pObj1, pObj2);
  iLength = CalcLength();
  // Seil als "Hilfsobjekt" verstecken
  SetCategory(C4D_StaticBack);
  SetObjDrawTransform(1, 0, 0, 0, 1, 0);
  while(Contained()) Exit();
  SetPosition(0,0);
}

private func GetPoint(int iIndex, int fGetY)
{
  if(fGetY) return aPointsY[iIndex];
  return aPointsX[iIndex];
}

private func GetPointNum()
{
  return GetLength(aPointsX);
}

private func AddPoint(int iX, int iY)
{
  var iLength = GetLength(aPointsX);
  aPointsX[iLength] = iX;
  aPointsY[iLength] = iY;
  return 1;
}

private func RemovePoint(int iIndex)
{
  if(iIndex >= GetLength(aPointsX)) return;
  DeleteArrayItem(iIndex, aPointsX);
  DeleteArrayItem(iIndex, aPointsY);
}

private func SetPoint(int iIndex, int iX, int iY)
{
  if(iIndex >= GetLength(aPointsX)) return;
  aPointsX[iIndex] = iX;
  aPointsY[iIndex] = iY;
  return 1;
}

public func CalcLength(pTarget1, pTarget2)
{
  // Anzahl Paare
  var i = GetPointNum() - 1;
  var iCount = 0;
  // für jedes Paar berechnen
  while(i-- > 0) iCount += VerticeLength(i, pTarget1, pTarget2);
  return iCount;
}

private func VerticeLength(i, pTarget1, pTarget2)
{
  // erster Vertex: Geschwindigkeit von 1.Actiontarget berücksichtigen
  if(i == 0) return Distance(
    GetPoint(0) + GetXDir(pTarget1) / 2, 
    GetPoint(0, 1) + GetYDir(pTarget1) / 2,
    GetPoint(1),
    GetPoint(1, 1));
  // vorletzter Vertex: Geschwindigkeit von 2.Actiontarget berücksichtigen
  if(i == (GetPointNum() - 2)) return Distance(
    GetPoint(i),
    GetPoint(i, 1),          
    GetPoint(i + 1) + GetXDir(pTarget2) / 2, 
    GetPoint(i + 1, 1) + GetYDir(pTarget2)) / 2;
  // Ansonsten nur die Positionen
  return Distance(
    GetPoint(i),
    GetPoint(i, 1),
    GetPoint(i + 1),
    GetPoint(i + 1, 1));
}

private func Timer()
{
  var i, iAmount, iSize = 10;
  // Partikel-Effekt
  while(i < GetPointNum() - 1)
  {
    var iAngle1 = Angle(GetPoint(i), GetPoint(i, 1), GetPoint(i + 1), GetPoint(i + 1, 1));
    var iAngle2 = iAngle1 - 180;
    var iX1 = GetPoint(i) - GetX() - Sin(iAngle1, 2);
    var iY1 = GetPoint(i, 1) - GetY() + Cos(iAngle1, 2);
    var iX2 = GetPoint(i + 1) - GetX() - Sin(iAngle2, 2);
    var iY2 = GetPoint(i + 1, 1) - GetY() + Cos(iAngle2, 2);
    iAmount += DrawParticleLine2("Rope", iX1, iY1, iX2, iY2, 5, 40, RGBa(100, 50, 0, 50));
    i++;
  }
}

private func DrawParticleLine2 (szKind, x0, y0, x1, y1, prtdist, a, b0, b1, iYDir)
{
  // Parameter gültig?
  if (!prtdist) return 0;
  // Anzahl der benötigten Partikel berechnen
  var prtnum = Max(Distance(x0, y0, x1, y1) / prtdist, 2);
  var i = prtnum;
  var iXDir = Sin(Angle(x0, y0, x1, y1), 10);
  iYDir = -Cos(Angle(x0, y0, x1, y1), 10);
  b1 = b0;
  var i1, i2, b;
  var szPart;
  // Partikel erzeugen!
  while(i>-1)
  {
    i2 = i * 256 / prtnum;
    i1 = 256 - i2;
    b =   ((b0&16711935)*i1 + (b1&16711935)*i2)>>8 & 16711935
        | ((b0>>8&16711935)*i1 + (b1>>8&16711935)*i2) & -16711936;
    if(!b && (b0 | b1)) ++b;
    szPart = szKind;
    if(i == prtnum) szPart = "Rope3";
    if(!i) szPart = "Rope2";
    CreateParticle(szPart, x0 + (x1 - x0) * i / prtnum, y0 + (y1 - y0) * i-- / prtnum, iXDir, iYDir, a, b, this);
  }
  // Erfolg; Anzahl erzeugter Partikel zurückgeben
  return prtnum;
}

func MoveRope(pObj1, pObj2)
{
  var a, b, d1, d2;
  // Wir brauchen mindesten 3 Vertices, damit die Algorithmen funktionieren
  if(GetPointNum () < 3)
    InsertPoint(1, (GetPoint(0) + GetPoint(1)) / 2, (GetPoint(0, 1) + GetPoint(1, 1)) / 2);
  SetRopeEnd(pObj1, 0, iVtx1, 2);
  SetRopeEnd(pObj2, GetPointNum () - 1, iVtx2, -2);
  var iVtxDist = 7 + 3 * GetPointNum();
  for(var i = GetPointNum() - 2; i > 0; i--)
  {
    // Neue Position des Vertices
    a = (GetPoint(i - 1) + GetPoint(i + 1)) / 2;
    b = (GetPoint(i - 1, 1) + GetPoint(i + 1, 1)) / 2 + 1;
    // Überschüssige Vertices, die nahe genug aneinander sind entfernen
    if(GetPointNum () > 3)
      if(Distance(GetPoint(i - 1, 0), GetPoint(i - 1, 1), GetPoint(i + 1, 0), GetPoint(i + 1, 1)) < iVtxDist)
        if(PathFree(GetPoint(i - 1, 0), GetPoint(i - 1, 1), GetPoint(i + 1, 0), GetPoint(i + 1, 1)))
          { RemovePoint(i); continue; }
    // bei zu langem Abstand einen neuen Vertex einfügen
    if(Distance(GetPoint(i - 1, 0), GetPoint(i - 1, 1), GetPoint(i, 0), GetPoint(i, 1)) > (7 + iVtxDist))
    {
      InsertPoint(i, (GetPoint(i - 1, 0) + GetPoint(i, 0)) / 2, (GetPoint(i - 1, 1) + GetPoint(i, 1)) / 2);
      continue;
    }
    // Wenn durch verschieben des Vertex das Seil durch solides ginge, nicht verschieben, und bei genug Platz einen zusätzlichen Vertex einfügen
    if(!PathFree(a, b, GetPoint(i - 1), GetPoint(i - 1, 1)))
    {
      if(Distance(GetPoint (i - 1, 0), GetPoint(i - 1, 1), GetPoint(i, 0), GetPoint(i, 1)) > iVtxDist)
        InsertPoint(i, (GetPoint(i - 1, 0) + GetPoint (i, 0)) / 2, (GetPoint(i - 1, 1) + GetPoint(i, 1)) / 2);
      continue;
    }
    if(!PathFree(a, b, GetPoint(i + 1), GetPoint(i + 1, 1)))
    {
      if(Distance(GetPoint(i + 1, 0), GetPoint(i + 1, 1), GetPoint(i, 0), GetPoint(i, 1)) > iVtxDist)
        InsertPoint(i + 1, (GetPoint(i + 1, 0) + GetPoint(i, 0)) / 2, (GetPoint(i + 1, 1) + GetPoint(i, 1)) / 2);
      continue;
    }
    SetPoint(i, a, b);
  }
  Timer();
}

func SetRopeEnd(pObj, iRopeVtx, iObjVtx, iWhichEnd)
{
  var x, y;
  x = GetX(pObj) + GetVertex(iObjVtx, 0, pObj);
  y = GetY(pObj) + GetVertex(iObjVtx, 1, pObj);
  // Wenn durch die Bewegung des angebundenen Objekts das Seil durch solides
  // ginge, einen Vertex an der letzten Position des Objekts einfügen
  // wenn das Objekt selbst in fester Materie steckt, hülfe das auch nichts
  if(!GBackSolid(x - GetX(), y - GetY()))
  {
    if(!PathFree(x, y, GetPoint(iRopeVtx + iWhichEnd, 0), GetPoint(iRopeVtx + iWhichEnd, 1)))
      InsertPoint(iRopeVtx - BoundBy(iWhichEnd, -1, 0), x, y);
    else
      SetPoint(iRopeVtx, x, y);
  }
}

private func InsertPoint(iIndex, x, y)
{
  if(!AddPoint()) return 0;
  for(var i = GetPointNum() - 1; i > iIndex; i--)
    SetPoint(i, GetPoint(i - 1, 0), GetPoint(i - 1, 1));
  SetPoint(iIndex, x, y);
  return 1;
}

func Connecting()
{
  // Wenn angeknüpfte Objekte verloren gehen: Seil löschen
  if(!GetActionTarget(0) || !GetActionTarget(1)) return RemoveObject();
  var pTarget1 = GetRealContainer(GetActionTarget(0));
  var pTarget2 = GetRealContainer(GetActionTarget(1));
  MoveRope(GetActionTarget(0), GetActionTarget(1));
  if(pTarget1 == pTarget2) return 0;
  iCalcedLength = CalcLength(pTarget1, pTarget2);  
  var iDifference = iCalcedLength - iLength;
  iLength = Max(iLength + GetActionTarget(0)->~RopeAskChangeLength(iDifference, this), 3);
  iLength = Max(iLength + GetActionTarget(1)->~RopeAskChangeLength(iDifference, this), 3);
  iDifference = iCalcedLength - iLength;
  if(iDifference <= 0) return 0;
  // bei zu kurzer Seillänge nicht mehr ziehen, um Aufschaukeln zu verhindern
  if(iCalcedLength < 5) return 0;
  // freie, verbundene Seilrollen ziehen sich nicht gegenseitig an
  if(GetID(pTarget1) == CL5P && GetID(pTarget2) == CL5P && !Contained(pTarget1) && !Contained(pTarget2))
    return 0;
  // wenn eine Seilrolle Seil gibt, dann in Ruhe lassen, außer, sie dämpft
  // den Fall von Clonks oder das Seil es einfach tun soll
  if(LocalN("iPull", pPulley) > 0 && Contained(pPulley))
    if(GetProcedure(Contained(pPulley)) != "FLIGHT")
      return 0;
  if(Local(8,this)) //Geheime lokahle.
    return 0;
  // Ansonsten Objekte zueinander bewegen
  PullObject((3 * GetPoint(1) + GetPoint(2)) / 4, (3 * GetPoint(1, 1) + GetPoint(2, 1)) / 4, iDifference, pTarget1, pTarget2, iVtx1);
  PullObject((GetPoint(GetPointNum() - 3) + 3 * GetPoint(GetPointNum() - 2)) / 4, (GetPoint(GetPointNum() - 3, 1) + 3 * GetPoint(GetPointNum() - 2, 1)) / 4, iDifference, pTarget2, pTarget1, iVtx2);

/*
  var iAngle = Angle(GetPoint(), GetPoint(0, 1), GetPoint(1), GetPoint(1, 1));
  var iX = Sin(iAngle, 8) + GetX(pTarget1);
  var iY = Cos(iAngle, 8) + GetY(pTarget1);
  PullObject(iX, iY, iDifference, pTarget2, pTarget1, iVtx1);
  SetLandscapePixel(iX, iY, RGB(1, 1, 1));

  iAngle = Angle(GetPoint(GetPointNum() - 1), GetPoint(GetPointNum() - 1, 1), GetPoint(GetPointNum() - 2), GetPoint(GetPointNum() - 2, 1));
  iX = Sin(iAngle, 8) + GetX(pTarget2);
  iY = Cos(iAngle, 8) + GetY(pTarget2);
  PullObject(iX, iY, iDifference, pTarget1, pTarget2, iVtx2);
  SetLandscapePixel(iX, iY, RGB(1, 1, 1));
*/
}

private func PullObject(iToX, iToY, iLength, pObj, pObj2, iVtx) // pObj wird versetzt
{
  if(!pObj || !pObj2) return 0;
  // Außnahmen:
  // will nicht :(
  if(pObj->~RejectPull(this)) return;
  // gefesselte Clonks lassen sich abführen
  if(GetEffect("IntTied", pObj2))
    return 0;
  // Gebäude (oder Inhaltsobjekte) sowie Hintergrundobjekte gar nicht ziehen
  if(GetCategory(pObj) & C4D_Structure || GetCategory(pObj) & C4D_StaticBack)
    return 0;
  // Objekt aus dem eigenen Grafikbereich heraus nicht ziehen, führt nur zu komischen Effekten
  for(var pObj3 in FindObjects(Find_AtPoint(GetX(pObj2) + GetVertex(pObj2->~GetVertexToConnect(), 0, pObj2), GetY(pObj2) + GetVertex(pObj2->~GetVertexToConnect(), 1, pObj2))))
    if(pObj3 == pObj)
      return 0;
  // Anker ziehen keine Schiffe nach unten
  if(pObj->~IsBoat() && GetActionTarget() && GetActionTarget(1))
  {
    if((GetActionTarget() == pObj && GetActionTarget(1)->~IsAnchor() && !Contained(GetActionTarget(1)))
    || (GetActionTarget(1) == pObj && GetActionTarget()->~IsAnchor() && !Contained(GetActionTarget())))
      return 0;
  }
  // Floateinfluss: z.B. Luftschiffe können heben
  if(GetProcedure(pObj) == "FLOAT")
    iLength = Max(0, iLength - (GetPhysical("Float", 0, pObj) / 20));

  var fFound;
  for(var x = 1; x <= 5; x++)
    if(GBackSolid(iToX + x - GetX(), iToY - GetY()))
    {
      iToX -= 7 - x;
      fFound = 7 - x;
      break;
    }
  if(!fFound)
  for(var x = 1; x <= 5; x++)
    if(GBackSolid(iToX - x - GetX(), iToY - GetY()))
    {
      iToX += 7 - x;
      fFound = 7 - x;
	    break;
    }
  var iLen = Min(iLength, Distance(iToX, iToY, GetX(pObj) + GetVertex(iVtx, 0, pObj), GetY(pObj) + GetVertex(iVtx, 1, pObj)));
  var iAngle = MakeThisAngleUseful(Angle(GetX(pObj) + GetVertex(iVtx, 0, pObj), GetY(pObj) + GetVertex(iVtx, 1, pObj), iToX, iToY), -180, +180);
  var iAngle2 = MakeThisAngleUseful(Angle(0, 0, GetVertex(iVtx, 0, pObj), GetVertex(iVtx, 1, pObj)), -180, +180);
  var x = Sin(iAngle, iLen * 20);
  var y = Cos(iAngle, iLen * 20);
  if(Stuck(pObj)) return 0;
  SetXDir(BoundBy(GetXDir(pObj, 100) + x, -1000, 1000), pObj, 100);
  SetYDir(BoundBy(GetYDir(pObj, 100) - y, -1000, 1000), pObj, 100);
  if(!InLiquid(pObj))
    SetRDir(GetRDir(pObj, 100) + (iAngle - iAngle2) / 10, pObj, 100);
  if(iLength > 30)
  SetPosition(GetX(pObj) + BoundBy(x, -1, 1), GetY(pObj) - BoundBy(y, -1, 1), pObj);
  //Falls Objekt dann feststecken würde, nicht wirklich versetzen
  if(Stuck(pObj))
    SetPosition(GetX(pObj) - BoundBy(x, -1, 1), GetY(pObj) + BoundBy(y, -1, 1), pObj);
 }

func MakeThisAngleUseful(iAngle, iM, iP)
{
  while(iAngle > iP) iAngle -= 360;
  while(iAngle < iM) iAngle += 360;
  return iAngle;
}

private func GetMiddlestVertex(pObj)
{
  var i, s = 1000, t, n;
  if(n = pObj->~GetVertexToConnect()) return n - 1;
  // alle Vertices durchgehen
  for(i = GetVertexNum(pObj); i >= 0; i--)
  {
    // x-Differenz zur Mitte + y-Differenz (eigentlich müsste ich Distance benutzen)
    t = (Abs(GetVertex(i, 0, pObj)) + Abs(GetVertex(i, 1, pObj)));
    //s: kürzester Abstand bis jetzt
    if(s > t)
    {
      //n: Vertex mit kürzestem Abstand
      s = t;
      n = i;
    }
  }
  return n;
}

// Ändert die angeknüpften Objekte
public func SetObject(pObj, iActionTarget)
{
  // 1. Actiontarget
  if(!iActionTarget)
  {
    // Action und 2.Actiontarget bleiben erhalten
    iVtx1 = GetMiddlestVertex(pObj);
    SetAction("Connect", pObj, GetActionTarget(1));
  }
  // 2.Actiontarget
  else
  {
    iVtx2 = GetMiddlestVertex(pObj);
    SetAction("Connect", GetActionTarget(), pObj);
  }
  return 1;
}

public func GetRopeLength() { return iLength; }
public func SetRopeLength(iNewLength) { return iLength = Max(iNewLength, 0); }

// nicht einsammelbar, wenn Hintergrundobjekt
private func RejectEntrance(pObj)
{
  if(GetCategory() & C4D_StaticBack)
    return 1;
  return _inherited(pObj, ...);
}

protected func Hit()
{
  Sound("WoodHit*");
  return 1;
}

func IsAnvilProduct() { return 1; }

global func GetRealContainer(pObj)
{
  var pRet = pObj;
  while(Contained(pRet)) pRet = Contained(pRet);
  return pRet;
}
