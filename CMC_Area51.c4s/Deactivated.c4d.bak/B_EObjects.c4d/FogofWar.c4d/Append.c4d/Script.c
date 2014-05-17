/*-- ClonkAppend für realistischen Kriegsnebel --*/

#strict 2
#appendto CLNK

local view;

public func Initialize()
{
  view = [];
  return(_inherited());
}

public func ResetView()
{
  var i = 0;
  while(view[i] != 0)
  {
    RemoveObject(view[i]);
    view[i] = 0;
    i++;
  }
  return(true);
}

public func SetRealisticView()
{
  if(GetPlayerType(GetOwner(this)) == C4PT_Script) return(true);
  SetPlrViewRange(0, this);
  SetFoW(true, GetOwner());
  if(FoW_OnlyCursor() && GetCursor(GetOwner()) != this)
  {
    ResetView();
    return(false);
  }
  if(GetAlive())
  {
    //Intelligente Berechnung
    //Funktionsprinzip 1: Wir errechnen die Mitte des Raums an dem Y-Wert des Clonks
    //Dann versuchen wir von dort aus in die Sichtrichtung den Raum zu füllen
    //So berechnet immer wieder Kugeln verschiedener Größen, bis eine Wand da ist
    var ViewsX = [];
    var ViewsY = [];
    var ViewsRange = [];
    var x0, y0, range0, x, y, range;
    x = x0 = GetX(this);
    y = y0 = GetVerticalMiddle(GetX(), GetClonkY());
    range = range0 = (GetVerticalBottom(x0, y0) - GetVerticalTop(x0, y0)) / 2;
    if(Contained()) {
        if(!FoW_CanSeeOutOfBuildings())
        {
	  ResetView();
          return(false);
        }
        else
        {
	  ViewsX[GetLength(ViewsX)] = x;
          ViewsY[GetLength(ViewsY)] = y;
          ViewsRange[GetLength(ViewsRange)] = range;
        }
    }
    else
    {
      var i = 0;
      if(FoW_CanSeeBehindBack() || GetDir(this) == DIR_Right)
      {
        while(
          PathFree(GetX(), GetClonkY(), x, y)
          && !(GBackSemiSolid(AbsX(x), AbsY(y)) && !GBackLiquid(AbsX(x), AbsY(y)))
          && x <= LandscapeWidth()
          && range >= FoW_MinimumRange()
          && i < 30)
        {
          ViewsX[GetLength(ViewsX)] = x;
          ViewsY[GetLength(ViewsY)] = y;
          ViewsRange[GetLength(ViewsRange)] = range;
          if(FoW_SightResolution())
          {
            x += Min(range, FoW_SightResolution())/2;
          }
          else
          {
            x += range/2;
          }
          y = GetVerticalMiddle(x, GetY());
          range = (GetVerticalBottom(x, GetY()) - GetVerticalTop(x, GetY())) / 2;
          i++;
        }
      }
      x = x0;
      y = y0;
      range = range0;
      var i = 0;
      if(FoW_CanSeeBehindBack() || GetDir(this) == DIR_Left)
      {
        while(
          PathFree(GetX(), GetClonkY(), x, y)
          && !(GBackSemiSolid(AbsX(x), AbsY(y)) && !GBackLiquid(AbsX(x), AbsY(y)))
          && x >= 0
          && range >= FoW_MinimumRange()
          && i < 30)
        {
          ViewsX[GetLength(ViewsX)] = x;
          ViewsY[GetLength(ViewsY)] = y;
          ViewsRange[GetLength(ViewsRange)] = range;
          if(FoW_SightResolution())
          {
            x -= Min(range, FoW_SightResolution());
          }
          else
          {
            x -= range;
          }
          y = GetVerticalMiddle(x, GetY());
          range = (GetVerticalBottom(x, GetY()) - GetVerticalTop(x, GetY())) / 2;
          i++;
        }
      }
    }
    var i = 0;
    while(ViewsX[i] != 0)
    {
      x = ViewsX[i];
      y = ViewsY[i];
      range = ViewsRange[i];
      if(!view[i])
      {
        view[i] = CreateObject(_VIW, 0, 0, GetOwner(this));
        view[i]->SetCategory(1);
        if(!FoW_Debug()) view[i]->SetVisibility(VIS_None);
      }
      if(view[i]->Contained()) view[i]->Exit();
      if(FoW_Debug()) Message(Format("%d", ViewsRange[i]), view[i]);
      SetPosition(x, y, view[i]);
      SetPlrViewRange(Max(range + FoW_Tolerance(), 50), view[i], true);
      i++;
    }
  }
  while(view[i] != 0)
  {
    RemoveObject(view[i]);
    i++;
  }
  return(true);
}

public func GetVerticalMiddle(x, y)
{
  var y1 = GetVerticalTop(x, y);
  var y2 = GetVerticalBottom(x, y);
  return((y2 - y1) / 2 + y1);
}

public func GetVerticalTop(x, y)
{
  if(PathFree(x, 0, x, y)) return(0);
  var Resolution = FoW_CalcResolution();
  //while (y > 0 && PathFree(x, y))
  while (y > 0 && !GBackSemiSolid(AbsX(x), AbsY(y)))
    y -= Resolution;
  return(y);  
}

public func GetVerticalBottom(x, y)
{
  if(PathFree(x, LandscapeHeight(), x, y)) return(LandscapeHeight());
  var Resolution = FoW_CalcResolution();
  //while (y < LandscapeHeight() && PathFree(x, y))
  while (y < LandscapeHeight() && !GBackSemiSolid(AbsX(x), AbsY(y)))
    y += Resolution;
  return(y);
}

protected func GetClonkY()
{
  var Offset = -5;
  while(GBackSemiSolid(AbsX(GetX()), AbsY(GetY()+Offset)) && Offset < 0)
  {
    Offset++;
  }
  return(GetY()+Offset);
}

protected func Death(a, b, c, d)
{
  ResetView();
  return(_inherited(a,b,c,d));
}