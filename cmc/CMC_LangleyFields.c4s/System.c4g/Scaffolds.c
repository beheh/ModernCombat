/*-- Aufbaugerüste --*/

#strict 2

global func CreateBuildScaffolds(object pBuilding)
{
  if(!pBuilding)
    return;

  var aScaffolds = [];
  var scaffoldid = pBuilding->~ScaffoldID(), buildingid = GetID(pBuilding);
  var xcount, xsize, xpos;
  var ycount, ysize, ypos;

  if(!scaffoldid)
    scaffoldid = SFFD;

  //Anzahl der Gerüste in X-Richtung
  xcount = GetDefWidth(buildingid)/GetDefWidth(scaffoldid);
  //Überstehende Pixel ermitteln
  xsize = GetDefWidth(buildingid)%GetDefWidth(scaffoldid);
  //Eventuell zusätzliches Gerüst einfügen
  if(!xcount || xsize > GetDefWidth(scaffoldid)/2)
    xcount++;
  //Vergrößerungsfaktor und neue Position berechnen
  xsize = ((GetDefWidth(buildingid)*1000)/xcount)/GetDefWidth(scaffoldid);
  xpos = GetDefWidth(scaffoldid)*xsize/2 - GetDefWidth(scaffoldid)*500;

  //Anzahl der Gerüste in Y-Richtung
  ycount = GetDefHeight(buildingid)/GetDefHeight(scaffoldid);
  //Überstehende Pixel ermitteln
  ysize = GetDefHeight(buildingid)%GetDefHeight(scaffoldid);
  //Eventuell zusätzliches Gerüst einfügen
  if(!ycount || ysize > GetDefHeight(scaffoldid)/2)
    ycount++;
  //Vergrößerungsfaktor und neue Position berechnen
  ysize = ((GetDefHeight(buildingid)*1000)/ycount)/GetDefHeight(scaffoldid);
  ypos = -(GetDefHeight(scaffoldid)*ysize/2 - GetDefHeight(scaffoldid)*500);

  for(var x = 0; x < xcount; x++)
    for(var y = 0; y < ycount; y++)
    {
      if(!aScaffolds[x])
      aScaffolds[x] = [];
      aScaffolds[x][y] = pBuilding->CreateObject(scaffoldid,(GetXOffset(buildingid) - GetXOffset(scaffoldid)) + (GetDefWidth(buildingid)*x)/xcount,
      													  -((GetYOffset(buildingid)*GetCon(pBuilding))/100)-(GetDefHeight(buildingid)*y)/ycount, GetOwner(pBuilding));
      SetObjDrawTransform(xsize,0,xpos,0,ysize,ypos,aScaffolds[x][y],0);
      aScaffolds[x][y]->AddVertex(0,((GetDefHeight(buildingid)*y)/ycount)-GetYOffset(scaffoldid)+1);
      SetObjectOrder(aScaffolds[x][y],pBuilding,1);
      aScaffolds[x][y]->~StartConstruction(x,y,xcount-1,ycount-1);
    }

  return aScaffolds;
}

global func RemoveBuildScaffolds(array &aScaffolds)
{
  for(var aTemp in aScaffolds)
      for(var scaffold in aTemp)
        if(scaffold)
          scaffold->~StartDestruction();
  aScaffolds = 0;
  return 1;
}
