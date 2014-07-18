/*-- Aufbauger�ste --*/

#strict 2

global func CreateBuildScaffolds(object pBuilding)
{
  if(!pBuilding)
    return;

  var aScaffolds = [];
  var scaffoldid = pBuilding->~ScaffoldID();
  var xcount, xsize, xpos;
  var ycount, ysize, ypos;

  if(!scaffoldid)
    scaffoldid = SFFD;

  //Anzahl der Ger�ste in X-Richtung
  xcount = GetDefWidth()/GetDefWidth(scaffoldid);
  //�berstehende Pixel ermitteln
  xsize = GetDefWidth()%GetDefWidth(scaffoldid);
  //Eventuell zus�tzliches Ger�st einf�gen
  if(!xcount || xsize > GetDefWidth(scaffoldid)/2)
    xcount++;
  //Vergr��erungsfaktor und neue Position berechnen
  xsize = ((GetDefWidth()*1000)/xcount)/GetDefWidth(scaffoldid);
  xpos = GetDefWidth(scaffoldid)*xsize/2 - GetDefWidth(scaffoldid)*500;

  //Anzahl der Ger�ste in Y-Richtung
  ycount = GetDefHeight()/GetDefHeight(scaffoldid);
  //�berstehende Pixel ermitteln
  ysize = GetDefHeight()%GetDefHeight(scaffoldid);
  //Eventuell zus�tzliches Ger�st einf�gen
  if(!ycount || ysize > GetDefHeight(scaffoldid)/2)
    ycount++;
  //Vergr��erungsfaktor und neue Position berechnen
  ysize = ((GetDefHeight()*1000)/ycount)/GetDefHeight(scaffoldid);
  ypos = -(GetDefHeight(scaffoldid)*ysize/2 - GetDefHeight(scaffoldid)*500);

  for(var x = 0; x < xcount; x++)
    for(var y = 0; y < ycount; y++)
    {
      if(!aScaffolds[x])
      aScaffolds[x] = [];
      aScaffolds[x][y] = CreateObject(scaffoldid,(GetXOffset() - GetXOffset(scaffoldid)) + (GetDefWidth()*x)/xcount,
      													  -((GetYOffset()*GetCon())/100)-(GetDefHeight()*y)/ycount);
      SetObjDrawTransform(xsize,0,xpos,0,ysize,ypos,aScaffolds[x][y],0);
      aScaffolds[x][y]->AddVertex(0,((GetDefHeight()*y)/ycount)-GetYOffset(scaffoldid)+1);
      SetObjectOrder(aScaffolds[x][y],this,1);
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