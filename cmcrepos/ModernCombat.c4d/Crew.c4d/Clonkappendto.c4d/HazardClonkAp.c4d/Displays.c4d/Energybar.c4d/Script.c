/*-- Energiebalken --*/

#strict

//GFXOV_MODE_ExtraGraphics

local target,controller,p;

protected func Initialize()
{
  return(1);
}

public func Set(object pTarget, id idIcon, int dwColor, object pController)
{
  if(!pController) pController = pTarget;
  if(!pTarget) return(RemoveObject());//:O
  
  if(!dwColor) dwColor = RGB(255,255,255);

  target = pTarget;
  controller = pController;
  
  SetOwner(GetOwner(pController));
  
  if(Hostile(GetOwner(pTarget),GetOwner(pController)))
    SetVisibility(VIS_All);
  else
    SetVisibility(VIS_Owner|VIS_Allies);
  
  SetGraphics("Bar", this(), GetID(), 1, 1);
  SetClrModulation(dwColor,this(),1);
  SetObjectBlitMode (GFX_BLIT_Additive,this(),1);
  
  if(idIcon)
    Message("@{{%i}}",this(),idIcon);
}

public func Update(int iP, int iG)
{
  p = BoundBy(iP*100/iG,0,100);
  //Timer();//Anzeige aktualisieren...
}

public func Remove()
{
  RemoveObject();
  /*if(!GetEffect("*FadeOut*", this()))
    FadeOut4K(13);*/
}

protected func Timer()
{
  if(!target) return(Remove());
  if(!controller) return(Remove());

  // Spieler weg?
  if(GetOwner() == NO_OWNER)
    return(Remove());
  
  SetPosition(GetX(target),GetY(target)-(GetDefHeight(GetID(target))/2 + 10));//Mit Attach währ besser...
 
  // Anzeige
  SetObjDrawTransform(p*10, 0, -1000 * (GetDefWidth()-(GetDefWidth()*p/100))/2, 0, 1000, 0, this(), 1);
}