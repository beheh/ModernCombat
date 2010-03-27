/*-- Leitergrafiken --*/

#strict
#appendto LADR


func Initialize()
{
  SetAction("2Be");
  ID = GetID();
  sizeX = GetDefCoreVal("Height",0,GetID())/2;
  sizeY = GetDefCoreVal("Width",0,GetID())/2;
  Graphics = AddEffect("LadderPartSave",this(),1,0,this(),GetID());
  ID = AddEffect("LadderPartSave",this(),1,0,this(),GetID());

  // Standardverhalten: nur Part1 anzeigen
  if(GetY() < 435) AddGraphics("Part1",LADR);
  else
  {
      AddGraphics(0,_LDR);
      AddGraphics("1",_LDR);
      AddGraphics("2",_LDR);
      AddGraphics("3",_LDR);
  }
  AddPart();
  AdjustSize();
}

public func UpdateTransferZone()
{
  ClearGraphics();
  if(GetY() < 300) AddGraphics("Part1",LADR);
  else
  {
   AddGraphics(0,_LDR);
   AddGraphics("1",_LDR);
   AddGraphics("2",_LDR);
   AddGraphics("3",_LDR);
  }
  AdjustSize();
  Redraw();
}