/*-- Gepackter Munitionsclip --*/

#strict

func ControlDig(object clonk)
{
  /*var ring = CreateRingMenu(0,clonk);
  
  var overlay;
  overlay = AddRingMenuItem(clonk,"BOING","Push",clonk,RICO);
  SetGraphics("1",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = AddRingMenuItem(clonk,"BOING","Push",clonk,RICO);
  SetGraphics("2",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = AddRingMenuItem(clonk,"BOING","Push",clonk,RICO);
  SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = AddRingMenuItem(clonk,"BOING","Push",clonk,RICO);
  SetGraphics("4",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = AddRingMenuItem(clonk,"BOING","Push",clonk,RICO);
  SetGraphics("5",ring,RICO,overlay,GFXOV_MODE_IngamePicture);*/
  
  var ring = CreateSpeedMenu(0,clonk);
  
  var overlay;
  overlay = ring->AddThrowItem("BOING","Push",clonk,RICO);
  SetGraphics("1",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = ring->AddUpItem("BOING","Push",clonk,RICO);
  SetGraphics("2",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = ring->AddRightItem("BOING","Push",clonk,RICO);
  SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = ring->AddDownItem("BOING","Push",clonk,RICO);
  SetGraphics("4",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = ring->AddLeftItem("BOING","Push",clonk,RICO);
  SetGraphics("5",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  return(1);
}

func Push(p)
{
  if(!p) return;
  
  Fling(p,(GetDir(p)*2-1) * 7, -5); 
}