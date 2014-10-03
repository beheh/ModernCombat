/*-- Leiter --*/

#strict 2

static const LADR_Left =	1;
static const LADR_Right =	2;
static const LADR_Front =	4;
static const LADR_Additional =	8;

local length;		//Anzahl Leiternteile
local sizeX, sizeY;
local Graphics, ID;	//Grafikeinstellung
local iGraCount;
local l_graphic;

public func IsClimbable()	{return true;}
public func GetGraphicsName()	{return l_graphic;}


/* Initialisierung */

protected func Initialize()
{
  l_graphic = "";
  SetAction("2Be");

  ID = GetID();
  sizeX = GetDefCoreVal("Height", 0, GetID()) / 2;
  sizeY = GetDefCoreVal("Width", 0, GetID()) / 2;
  Graphics = AddEffect("LadderPartSave", this, 1, 0, this, GetID());
  ID = AddEffect("LadderPartSave", this, 1, 0, this, GetID());

  //Standardverhalten: nur Part1 anzeigen
  AddGraphics("Part1", LADR);
  AddPart();
  AdjustSize();
}

public func AddGraphics(string sNewGraphic, id idNewID)
{
  sNewGraphic = Format("%s%s", sNewGraphic, l_graphic);
  EffectCall(this, Graphics, "AddPart", sNewGraphic, iGraCount);
  EffectCall(this, ID, "AddPart", idNewID, iGraCount);
  iGraCount++;
}

public func ClearGraphics()
{
  while(iGraCount >= 0)
  {
    EffectCall(this, Graphics, "ClearParts", iGraCount);
    EffectCall(this, ID, "ClearParts", iGraCount);
    iGraCount--;
  }
  iGraCount = 0;
}

public func SetSize(int x,int y)
{
  sizeX = x;
  sizeY = y;
  AdjustSize();
}

/* Einstellung */

public func Set(int iNewLength, int iDir, string szGraphic)
{
  if(iNewLength <= 0)
    return 0;

  if(!szGraphic)
    szGraphic = "";

  if(l_graphic != szGraphic)
  {
    l_graphic = szGraphic;

    ClearGraphics();
    AddGraphics("Part1", LADR);

    Redraw();
  }

  var cmd;
  if(iDir)
  {
    ClearGraphics();

    if(iDir & LADR_Left) AddGraphics("Part2",LADR);
    if(iDir & LADR_Right) AddGraphics("Part3",LADR);
    if(iDir & LADR_Front) AddGraphics("Part1",LADR);
    if(iDir & LADR_Additional)
    {
      var graphcount = 4;

      while(SetGraphics(Format("Part%d%s", graphcount, szGraphic), this))
        graphcount++;

      SetGraphics("", this);
      for(var i = 4; i < graphcount; i++)
        AddGraphics(Format("Part%d", i), LADR);
    }
    Redraw();
    return Set(iNewLength, 0, szGraphic);
  }
  //Falls neue Länge größer als momentane
  if(length < iNewLength)
    cmd = "Add"; //hinzufügen
  //Falls kleiner
  if(length > iNewLength)
    cmd = "Remove"; //löschen

  for(var x = Abs(length-iNewLength); x > 0; x--)
    PrivateCall(this, Format("%sPart", cmd));
  //Letztlich Größe anpassen
  AdjustSize();
  return 1;
}

/* Grafik aktualisieren */

public func Redraw()
{
  for(var x = 1; x <= length; x++)
  {
    var graph = Random(iGraCount);
    SetGraphics(EffectCall(this, Graphics, "GetPart", graph), this, EffectCall(this, ID, "GetPart", graph), x, GFXOV_MODE_Base);
    SetObjDrawTransform(1000, 0, 0, 0, 1000, 1000*sizeY*(x-1), this, x);
  }
}

private func AddPart()
{
  //Ein Teil mehr, Länge raufzählen
  length++;
  var graph = Random(iGraCount);
  if(!SetGraphics(EffectCall(this, Graphics, "GetPart", graph), this, EffectCall(this, ID, "GetPart", graph), length, GFXOV_MODE_Base))
    return 0;

  //Position bestimmen
  SetObjDrawTransform(1000, 0, 0, 0, 1000, 1000*sizeY*(length-1), this, length);
  return 1;
}

private func RemovePart()
{
  if(length < 0)
    return 0;

  //letztes Teil entfernen
  SetGraphics(0, 0, 0, length);
  //ein Teil weniger, Länge runterzählen
  length--;
  return 1;
}

private func AdjustSize()
{
  var ydiff = GetObjHeight();
  SetShape(-sizeX, -sizeY, sizeX*2, sizeY+sizeY*length, this);
  ydiff -= GetObjHeight();
  SetPosition(GetX(), GetY()+ydiff);
  SetVertex(0, 1, -sizeY, this);
  SetVertex(1, 1, sizeY*length, this);

  //Positionen aller Teile anpassen
  for(var x = 1; x <= length; x++)
    SetObjDrawTransform(1000, 0, 0, 0, 1000, sizeY*1000*(x-1), this, x);
}

private func FxLadderPartSaveAddPart(object pTarget, int iEffectNr, string graphic, int index)
{
  var length;
  if(!(length = index))
    while(EffectVar(length, pTarget, iEffectNr))
      length++;

  EffectVar(length, pTarget, iEffectNr) = graphic;
}

private func FxLadderPartSaveClearParts(object pTarget, int iEffectNr, int index)
{
  EffectVar(index, pTarget, iEffectNr) = 0;
}

private func FxLadderPartSaveGetPart(object pTarget, int iEffectNr, int index)
{
  return EffectVar(index, pTarget, iEffectNr);
}

public func UpdateTransferZone()
{
  ClearGraphics();

  for(var z = 1; z <= 3; z++)
    AddGraphics(Format("Part%d", z), LADR);

  AdjustSize();
  Redraw();
}

/* Serialisierung */

public func Serialize(array& extra, int& extray)
{
  extra[GetLength(extra)] = Format("Set(%d)", length);
  extray = -(length - 1) * sizeY;
}