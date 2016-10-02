/*-- Seil --*/

#strict 2

local iLength, iCalcedLength, iOCF, iVtx1, iVtx2;
local aPointsX, aPointsY;
local CntObj1, CntObj2;
local pPulley;
local fNoPickUp_0, fNoPickUp_1;
local dwcolor;
local fStaticRope, fDisablePhysics, fZipline;

public func SetRopeColor(int dwClr)	{return dwcolor = dwClr;}
public func GetRopeLength()		{return iLength;}
public func SetRopeLength(iNewLength)	{return iLength = Max(iNewLength, 0);}
public func IsStatic()			{return fStaticRope;}
public func NoPhysics()			{return fDisablePhysics;}
public func IsZipline()			{return fZipline && NoPhysics();} 
func IsAnvilProduct()			{return 1;}


/* Seilrutsche */

global func CreateZipLine(int iX1, int iY1, int iX2, int iY2)
{
  //Knotenpunkte setzen
  var np1 = CreateObject(NLPT, iX1, iY1, -1);
  var np2 = CreateObject(NLPT, iX2, iY2, -1);

  //Seil erstellen und konfigurieren
  var rope = CreateObject(CK5P, 0, 0, -1);
  rope->ConnectObjects(np1, np2);
  rope->SetStaticMode(true);
  rope->SetZipline(true);

  return rope;
}

/* Zerstörung */

protected func Destruction()
{
  //Absichtlich nicht IsZipline, damit die Knotenpunkte auch entfernt werden und
  //auch bei NoPhysics die Nodepoints gelöscht
  if(fZipline)
  {
    if(CntObj1)
      RemoveObject(CntObj1);
    if(CntObj2)
      RemoveObject(CntObj2);
  }
  return _inherited(...);
}

/* Seil abtrennen */

public func CutRope(int iPosition, int iPlrPlusOne)
{
  //Höhenunterschiede berücksichtigen
  if(iPosition && GetPoint(0, true) > GetPoint(GetPointNum()-1, true))
    iPosition = iPosition+((!(iPosition-1))*2-1);

  SetZipline(false);
  SetStaticMode(false);

  var at0 = GetActionTarget(), at1 = GetActionTarget(1);
  var angle = Angle(GetX(at0), GetY(at0), GetX(at1), GetY(at1));
  var temp1, temp2;

  if(!iPosition || iPosition == 1)
  {
    temp1 = CutRopeEnd(0, angle, iPlrPlusOne);

    if(iPosition)
    {
      ConnectObjects(at1, temp1);
      SetXDir(+Sin(angle, 100)/2, temp1);
      SetYDir(-Cos(angle, 100)/2, temp1);
    }
  }
  if(!iPosition || iPosition == 2)
  {
    temp2 = CutRopeEnd(true, angle+180, iPlrPlusOne);

    if(iPosition)
    {
      ConnectObjects(at0, temp2);
      SetXDir(+Sin(angle+180, 100)/2, temp2);
      SetYDir(-Cos(angle+180, 100)/2, temp2);
    }
  }

  if(!iPosition)
  {
    ConnectObjects(temp1, temp2);
    SetXDir(+Sin(angle, 100)/3, temp1);
    SetYDir(-Cos(angle, 100)/3, temp1);
    SetXDir(+Sin(angle+180, 100)/3, temp2);
    SetYDir(-Cos(angle+180, 100)/3, temp2);
  }

  //Verschwinden
  FadeOut();

  return true;
}

private func CutRopeEnd(bool fEnd, int iAngle, int iPlrPlusOne)
{
  var index = 0;
  if(fEnd)
    index = GetPointNum()-1;

  var xoff, yoff;
  xoff = Sin(iAngle, GetDefWidth(NLPT), 100);
  yoff = -Cos(iAngle, GetDefHeight(NLPT), 100);

  //Temporäres Knotenpunktobjekt erzeugen
  var temp = CreateObject(NLPT, GetPoint(index)+xoff, GetPoint(index, true)+yoff, iPlrPlusOne-1);
  temp->SetCategory(C4D_Object);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(250,0,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",6,15,0,0,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));

  FadeOut(temp);
  return temp;
}

public func SetZipline(bool fSet)
{
  fZipline = fSet;

  if(fZipline)
  {
    SetRopeColor(RGB(50,50,50));
    DisablePhysics(true);

    //Seil muss gerade sein
    if(WildcardMatch(GetAction(), "*Connect*"))
    {
      var o1 = GetActionTarget(), o2 = GetActionTarget(1);
      aPointsX = [0, 0];
      aPointsY = [0, 0];

      ConnectObjects(o1, o2);
    }

    SetRopeLength(Distance(GetPoint(), GetPoint(0, true), GetPoint(GetPointNum()-1), GetPoint(GetPointNum()-1, true)));
  }
  else
    DisablePhysics(false);

  return true;
}

public func SetStaticMode(bool fSet)
{
  fStaticRope = fSet;
  if(fStaticRope)
    SetAction("ConnectStatic", GetActionTarget(), GetActionTarget(1));
  if(!fStaticRope)
  {
    //Alte Partikel entfernen
    ClearParticles("RopeStatic");
    ClearParticles("Rope2Static");

    SetAction("Connect");

    //Alle statischen Seile zum Neuzeichnen auffordern
    for(var obj in FindObjects(Find_ID(CK5P), Find_Func("IsStatic"), Find_Action("ConnectStatic")))
      obj->Connecting();
  }

  return true;
}

public func DisablePhysics(bool fSet)
{
  fDisablePhysics = fSet;
  return fSet;
}

/* Initialisierung */

protected func Initialize()
{
  //Standardfarbe des Seils
  dwcolor = RGBa(100, 50, 0, 50);

  // OCF's, an die verbunden werden kann
  iOCF = OCF_Living | OCF_Grab | OCF_Chop | OCF_Collectible;

  aPointsX = [0,0];
  aPointsY = [0,0];
  return 1;
}

/* Verbinden */

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
  if(IsStatic())
    SetAction("ConnectStatic", pObj1, pObj2);
  else
    SetAction("Connect", pObj1, pObj2);
  iLength = CalcLength();

  //Seil als Hilfsobjekt verstecken
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
  //Anzahl Paare
  var i = GetPointNum() - 1;
  var iCount = 0;
  //für jedes Paar berechnen
  while(i-- > 0) iCount += VerticeLength(i, pTarget1, pTarget2);
  return iCount;
}

private func VerticeLength(i, pTarget1, pTarget2)
{
  //Erster Vertex: Geschwindigkeit von erstem Actiontarget berücksichtigen
  if(i == 0) return Distance(
    GetPoint(0) + GetXDir(pTarget1) / 2, 
    GetPoint(0, 1) + GetYDir(pTarget1) / 2,
    GetPoint(1),
    GetPoint(1, 1));
  //Vorletzter Vertex: Geschwindigkeit von  zweitem Actiontarget berücksichtigen
  if(i == (GetPointNum() - 2)) return Distance(
    GetPoint(i),
    GetPoint(i, 1),
    GetPoint(i + 1) + GetXDir(pTarget2) / 2, 
    GetPoint(i + 1, 1) + GetYDir(pTarget2)) / 2;
  //Ansonsten nur die Positionen
  return Distance(
    GetPoint(i),
    GetPoint(i, 1),
    GetPoint(i + 1),
    GetPoint(i + 1, 1));
}

private func Timer()
{
  var i, iAmount;
  //Partikeleffekt
  while(i < GetPointNum() - 1)
  {
    var iAngle1 = Angle(GetPoint(i), GetPoint(i, 1), GetPoint(i + 1), GetPoint(i + 1, 1));
    var iAngle2 = iAngle1 - 180;
    var iX1 = GetPoint(i) - GetX() - Sin(iAngle1, 2);
    var iY1 = GetPoint(i, 1) - GetY() + Cos(iAngle1, 2);
    var iX2 = GetPoint(i + 1) - GetX() - Sin(iAngle2, 2);
    var iY2 = GetPoint(i + 1, 1) - GetY() + Cos(iAngle2, 2);
    var partName = "Rope";
    if(IsStatic())
      partName = "RopeStatic";
    iAmount += DrawParticleLine2(partName, iX1, iY1, iX2, iY2, 5, 40, dwcolor);
    i++;
  }
}

/* Seil zeichnen */

private func DrawParticleLine2 (szKind, x0, y0, x1, y1, prtdist, a, b0, b1, iYDir)
{
  //Parameter gültig?
  if (!prtdist) return 0;
  //Anzahl der benötigten Partikel berechnen
  var prtnum = Max(Distance(x0, y0, x1, y1) / prtdist, 2);
  var i = prtnum;
  var iXDir = Sin(Angle(x0, y0, x1, y1), 10);
  iYDir = -Cos(Angle(x0, y0, x1, y1), 10);
  b1 = b0;
  var i1, i2, b;
  var szPart;
  //Partikel erzeugen
  while(i>-1)
  {
    i2 = i * 256 / prtnum;
    i1 = 256 - i2;
    b =   ((b0&16711935)*i1 + (b1&16711935)*i2)>>8 & 16711935
        | ((b0>>8&16711935)*i1 + (b1>>8&16711935)*i2) & -16711936;
    if(!b && (b0 | b1)) ++b;
    szPart = szKind;
    if(i == prtnum) szPart = "Rope3";
    if(!i)
    {
      szPart = "Rope2";
      if(IsStatic())
        szPart = "Rope2Static";
    }
    var obj = this;
    if(IsStatic())
      obj = 0;
    CreateParticle(szPart, x0 + (x1 - x0) * i / prtnum, y0 + (y1 - y0) * i-- / prtnum, iXDir, iYDir, a, b, obj);
  }
  //Erfolg; Anzahl erzeugter Partikel zurückgeben
  return prtnum;
}

func MoveRope(pObj1, pObj2)
{
  if(fDisablePhysics)
    return Timer();

  var a, b;
  //Mindestens 3 Vertices, damit die Algorithmen funktionieren
  if(GetPointNum () < 3)
    InsertPoint(1, (GetPoint(0) + GetPoint(1)) / 2, (GetPoint(0, 1) + GetPoint(1, 1)) / 2);
  SetRopeEnd(pObj1, 0, iVtx1, 2);
  SetRopeEnd(pObj2, GetPointNum () - 1, iVtx2, -2);
  var iVtxDist = 7 + 3 * GetPointNum();
  for(var i = GetPointNum() - 2; i > 0; i--)
  {
    //Neue Position des Vertices
    a = (GetPoint(i - 1) + GetPoint(i + 1)) / 2;
    b = (GetPoint(i - 1, 1) + GetPoint(i + 1, 1)) / 2 + 1;
    //Überschüssige Vertices, die nahe genug aneinander sind, entfernen
    if(GetPointNum () > 3)
      if(Distance(GetPoint(i - 1, 0), GetPoint(i - 1, 1), GetPoint(i + 1, 0), GetPoint(i + 1, 1)) < iVtxDist)
        if(PathFree(GetPoint(i - 1, 0), GetPoint(i - 1, 1), GetPoint(i + 1, 0), GetPoint(i + 1, 1)))
          { RemovePoint(i); continue; }
    //Bei zu langem Abstand einen neuen Vertex einfügen
    if(Distance(GetPoint(i - 1, 0), GetPoint(i - 1, 1), GetPoint(i, 0), GetPoint(i, 1)) > (7 + iVtxDist))
    {
      InsertPoint(i, (GetPoint(i - 1, 0) + GetPoint(i, 0)) / 2, (GetPoint(i - 1, 1) + GetPoint(i, 1)) / 2);
      continue;
    }
    //Wenn durch Verschieben des Vertex das Seil durch solides ginge, nicht verschieben, und bei genug Platz einen zusätzlichen Vertex einfügen
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
  //Wenn durch die Bewegung des angebundenen Objekts das Seil durch solides
  //ginge, einen Vertex an der letzten Position des Objekts einfügen
  //wenn das Objekt selbst in fester Materie steckt, hülfe das auch nichts
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
  //Wenn angeknüpfte Objekte verloren gehen: Seil löschen
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
  //Bei zu kurzer Seillänge nicht mehr ziehen, um Aufschaukeln zu verhindern
  if(iCalcedLength < 5) return 0;
  // freie, verbundene Seilrollen ziehen sich nicht gegenseitig an
  if(GetID(pTarget1) == CL5P && GetID(pTarget2) == CL5P && !Contained(pTarget1) && !Contained(pTarget2))
    return 0;
  //Wenn eine Seilrolle Seil gibt, dann in Ruhe lassen, außer, sie dämpft
  //Den Fall von Clonks oder das Seil es einfach tun soll
  if(LocalN("iPull", pPulley) > 0 && Contained(pPulley))
    if(GetProcedure(Contained(pPulley)) != "FLIGHT")
      return 0;
  if(Local(8,this))
    return 0;
  //Ansonsten Objekte zueinander bewegen
  PullObject((3 * GetPoint(1) + GetPoint(2)) / 4, (3 * GetPoint(1, 1) + GetPoint(2, 1)) / 4, iDifference, pTarget1, pTarget2, iVtx1);
  PullObject((GetPoint(GetPointNum() - 3) + 3 * GetPoint(GetPointNum() - 2)) / 4, (GetPoint(GetPointNum() - 3, 1) + 3 * GetPoint(GetPointNum() - 2, 1)) / 4, iDifference, pTarget2, pTarget1, iVtx2);
}

private func PullObject(iToX, iToY, iLength, pObj, pObj2, iVtx)	//pObj wird versetzt
{
  if(fDisablePhysics) return;
  if(!pObj || !pObj2) return 0;
  if(pObj->~RejectPull(this)) return;
  //Gefesselte Clonks lassen sich abführen
  if(GetEffect("IntTied", pObj2))
    return 0;
  //Gebäude (oder Inhaltsobjekte) sowie Hintergrundobjekte gar nicht ziehen
  if(GetCategory(pObj) & C4D_Structure || GetCategory(pObj) & C4D_StaticBack)
    return 0;
  //Objekt aus dem eigenen Grafikbereich heraus nicht ziehen
  for(var pObj3 in FindObjects(Find_AtPoint(GetX(pObj2) + GetVertex(pObj2->~GetVertexToConnect(), 0, pObj2), GetY(pObj2) + GetVertex(pObj2->~GetVertexToConnect(), 1, pObj2))))
    if(pObj3 == pObj)
      return 0;
  //Anker ziehen keine Schiffe nach unten
  if(pObj->~IsBoat() && GetActionTarget() && GetActionTarget(1))
  {
    if((GetActionTarget() == pObj && GetActionTarget(1)->~IsAnchor() && !Contained(GetActionTarget(1)))
    || (GetActionTarget(1) == pObj && GetActionTarget()->~IsAnchor() && !Contained(GetActionTarget())))
      return 0;
  }
  //Float-Einfluss: z.B. Luftschiffe können heben
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
  //Falls das Objekt dann feststecken würde, nicht wirklich versetzen
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
  //Alle Vertices durchgehen
  for(i = GetVertexNum(pObj); i >= 0; i--)
  {
    //x-Differenz zur Mitte + y-Differenz
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

public func SetObject(pObj, iActionTarget)
{
  //1. Actiontarget
  if(!iActionTarget)
  {
    //Action und 2.Actiontarget bleiben erhalten
    iVtx1 = GetMiddlestVertex(pObj);
    if(IsStatic())
      SetAction("ConnectStatic", pObj, GetActionTarget(1));
    else
      SetAction("Connect", pObj, GetActionTarget(1));
  }
  //2.Actiontarget
  else
  {
    iVtx2 = GetMiddlestVertex(pObj);
    if(IsStatic())
      SetAction("ConnectStatic", GetActionTarget(), pObj);
    else
      SetAction("Connect", GetActionTarget(), pObj);
  }
  return 1;
}

/* Einsammeln als Hintergrundobjekt verhindern */

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

global func GetRealContainer(pObj)
{
  var pRet = pObj;
  while(Contained(pRet)) pRet = Contained(pRet);
  return pRet;
}