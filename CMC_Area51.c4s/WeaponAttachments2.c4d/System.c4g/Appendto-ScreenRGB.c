/*-- Neues Script --*/

#strict 2

#appendto S24K

public func Set(object pTarget, int dwRGBa, int iAlphaAdd, int iFadeRate, bool bAdditive, int iLayer, int iMin)
{
  target = pTarget;
  layer = iLayer;

  if(target && GetCategory(target) & C4D_Living && !(GetOCF(target) & OCF_Alive))
    return RemoveObject();

  if(!target)
    SetVisibility(VIS_All);
  else
    SetAction("CursorCheck");

  //Additiv zeichnen?
  if(bAdditive)
    SetObjectBlitMode(GFX_BLIT_Additive);

  fade = iFadeRate;

  var a_save = a;
  SplitRGBaValue(dwRGBa,r,g,b,a);
  if(a_save < 255 || !a)
  	a = BoundBy(a_save-iAlphaAdd,iMin,255);

  if(!fade)
  {
    RemoveEffect("IntRGBFade",this);
    SetClrModulation(RGBa(r, g, b, a));
  }
  else
    if(!GetEffect("IntRGBFade",this))
      AddEffect("IntRGBFade",this,1,3,this);
}
