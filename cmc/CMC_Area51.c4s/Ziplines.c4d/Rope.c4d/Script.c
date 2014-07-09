/*-- Seil --*/

#strict 2

local iLength, iCalcedLength, iOCF, iVtx1, iVtx2;
local aPointsX, aPointsY;
local pPulley;
local fNoPickUp_0, fNoPickUp_1;
local dwcolor;
local fStaticRope, fDisablePhysics, fZipline;

public func SetRopeColor(int dwClr)	{return dwcolor = dwClr;}
public func GetRopeLength()		{return iLength;}
public func SetRopeLength(iNewLength)	{return iLength = Max(iNewLength, 0);}
public func IsStatic()		{ return fStaticRope; }
public func NoPhysics()   { return fDisablePhysics; }
public func IsZipline()   { return fZipline && NoPhysics(); } 
func IsAnvilProduct()			{return 1;}

global func CreateZipline(int iX1, int iY1, int iX2, int iY2)
{
	var np1 = CreateObject(NLPT, iX1, iY1, -1);
  var np2 = CreateObject(NLPT, iX2, iY2, -1);
  var rope = CreateObject(CK5P, 0, 0, -1);
  
  rope->ConnectObjects(np1, np2);
  rope->SetStaticMode(true);
  rope->SetZipline(true);
  
  return true;
} 

//Trennt das Seil am Anfang/Ende ab 
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

  //Partikel- und Soundeffekte
  //...

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