/*-- Schwerverletzter --*/

#strict

local clonk,oldvisrange,oldvisstate;

public func Set(object pClonk)
{
  clonk = pClonk;
  SetPosition(GetX(pClonk),GetY(pClonk)/*GetObjHeight(pClonk)/2*/);
  SetXDir(GetXDir(pClonk));
  SetYDir(GetYDir(pClonk));
  
  pClonk->Enter(this());//Schlucken.
  GrabContents(pClonk);
  SetGraphics (0,this(),GetID(pClonk),1,GFXOV_MODE_Object,0,0,pClonk);//Darstellen.
  
  //Bei der Initialisierung die Werte speichern.
  oldvisrange = GetObjPlrViewRange(pClonk);
  oldvisstate = GetPlrFogOfWar(GetOwner(pClonk));
  
  SetPlrViewRange(100,pClonk);
  SetFoW(true,GetOwner(pClonk)); 
  
  //DeathMenu();
}

func DeathMenu()
{
  CreateMenu (FKDT, clonk, this(), 0, GetName(), 0, C4MN_Style_Dialog, false);
  AddMenuItem ("$Suicide$", "Suicide", SKUL,clonk, 0, 0, "$DescSuicide$");

  //SetMenuDecoration(MCDC, pClonk);
  SetMenuTextProgress(1, clonk); 
}

public func Suicide()
{
  clonk->Kill();
}

public func GetClonk() {return(clonk);}

public func Destruction()
{
  if(!clonk) return();
  
  if(Contained(clonk) == this())
    clonk->Exit(0,0,GetObjHeight(clonk)/2);
    
  if(GetAlive(clonk))
    clonk->GrabContents(this());
  else
    while(Contents())
      Exit(Contents(),0,+10);
      
  //Sichtdaten zurücksetzen.
  SetFoW(oldvisstate,GetOwner(clonk));
  SetPlrViewRange(oldvisrange,clonk);
}

public func MenuQueryCancel() { return(true); }

public func RejectCollect(id idObj, object pObj)
{
  if(!clonk) return();
  var val = clonk->~RejectCollect(idObj,pObj);
  if(val)  Message("RejectCollect",this());
  return(val);
}

//Tags
public func AimAngle()     {return();}
public func ReadyToFire()  {return();}
public func IsAiming()     {return();}
