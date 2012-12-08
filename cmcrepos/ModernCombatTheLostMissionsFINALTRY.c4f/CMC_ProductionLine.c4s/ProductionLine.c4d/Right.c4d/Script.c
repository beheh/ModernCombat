/*-- Flieﬂbandteil --*/

#strict 2
#include PM1C


func CheckEnds(dir)
{
  var objects = FindObjects(Find_InRect(10, -18, 30, 30), Find_Category(C4D_Living), Find_Or(Find_Action("Scale"), Find_Action("ScaleDown")));
  for(object in objects)
    SetYDir(30*dir, object);

  return true;
}