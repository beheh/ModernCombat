/*-- Schwerverletzter --*/

#strict 2

local clonk;

public func Set(object pClonk)
{
  clonk = pClonk;
  SetPosition(GetX(pClonk),GetY(pClonk)/*GetObjHeight(pClonk)/2*/);
  SetXDir(GetXDir(pClonk));
  SetYDir(GetYDir(pClonk));
  
  for(var a in FindObjects(Find_ActionTarget(this),Find_NoContainer()))
  {
    Log(GetName(a));
    
  }
  
  pClonk->Enter(this);//Schlucken.
  GrabContents(pClonk);
  SetGraphics (0,this,GetID(pClonk),1,GFXOV_MODE_Object,0,0,pClonk);//Darstellen.
  
  SetPlrViewRange (100);
  
  DeathMenu();
}

func DeathMenu() {

  ShiftGrenades();
  
  CreateMenu (FKDT, clonk, this, 0, GetName(), 0, C4MN_Style_Dialog, false);
  AddMenuItem ("$Suicide$", "Suicide", SKUL,clonk, 0, 0, "$DescSuicide$");
  
  if(Contents()->~IsGrenade()) {
    AddMenuItem ("$ThrowRight$", "ThrowRight", FRAG, clonk, 0, 0, "$DescThrow$");
    AddMenuItem ("$ThrowLeft$", "ThrowLeft", FRAG, clonk, 0, 0, "$DescThrow$");
  }
  //SetMenuDecoration(MCDC, pClonk);
  SetMenuTextProgress(1, clonk); 
  }

func ShiftGrenades() {
  for(var i; i < ContentsCount(); ++i) {
  if(!Contents()->~IsGrenade()) ShiftContents();
  }
  }

public func Suicide()
{
  clonk->Kill();
}

public func GetClonk(){return clonk;}

public func Destruction()
{
  if(!clonk) return ;
  
  if(Contained(clonk) == this)
    clonk->Exit(0,0,GetObjHeight(clonk)/2);
    
  if(GetAlive(clonk))
    clonk->GrabContents(this);
  else
    while(Contents())
      Exit(Contents(),0,+10);
  
  /*SetObjDrawTransform
  (
    1000,0,0,
    0,1000,0,
    clonk
  );*/
}

public func MenuQueryCancel() { return true; }

public func RejectCollect(id idObj, object pObj)
{
  if(!clonk) return ;
  var val = clonk->~RejectCollect(idObj,pObj);
  if(val)  Message("RejectCollect",this);
  return val;
}

func FxReloadTimer() { return -1; } 

func ThrowRight() {
  if(!GetEffect("Reload", clonk)) {
  Contents()->~Fuse();
  Exit(Contents(), -5, 0, 0, 3, 3);
  AddEffect("Reload", clonk, 1, 200);
  }
  DeathMenu();
  }
 
func ThrowLeft() {
  if(!GetEffect("Reload", clonk)) {
  Contents()->~Fuse();
  Exit(Contents(), -5, 0, 0, -3, -3);
  AddEffect("Reload", clonk, 1, 200);
  }
  DeathMenu();
  }

//Tags
public func AimAngle()     {return ;}
public func ReadyToFire()  {return ;}
public func IsAiming()     {return ;}
