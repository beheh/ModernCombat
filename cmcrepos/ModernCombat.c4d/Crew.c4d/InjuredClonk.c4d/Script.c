/*-- Schwerverletzter --*/

#strict

local clonk,oldvisrange,oldvisstate,suicide;

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
  
  suicide = 30;
  ScheduleCall(this(),"DoMenu",35,suicide); 
}

func DoMenu()
{
  suicide--;
  DeathMenu();
}

func DeathMenu()
{
  CloseMenu(clonk);
  CreateMenu (FKDT, clonk, this(), 0, GetName(), 0, C4MN_Style_Dialog, true);
  if(suicide > 0)
    AddMenuItem(Format("<c %X>$SuicideW$</c>",RGB(128,128,128),suicide), "NoSuicide", SKUL,clonk, 0, 0, "$DescSuicide$");
  else
    AddMenuItem("$Suicide$", "Suicide", SKUL,clonk, 0, 0, "$DescSuicide$");

  //SetMenuDecoration(MCDC, pClonk);
  SetMenuTextProgress(1, clonk); 
}

public func NoSuicide()
{
  Sound("ERROR",true,0,0,GetOwner(clonk)+1);
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

public func ControlDig(object pCaller)
{
  if(pCaller == clonk) return(1);
  SetCommand(pCaller,"Context",0,0,0,this());
  return(1);
}

//Tags
public func AimAngle()     {return();}
public func ReadyToFire()  {return();}
public func IsAiming()     {return();}
