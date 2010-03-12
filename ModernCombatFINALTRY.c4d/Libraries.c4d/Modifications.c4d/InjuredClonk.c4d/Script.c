/*-- Schwerverletzter --*/

#strict

static const FKDT_SuicideTime = 15; //Standardzeit bei Fake Death

local clonk,oldvisrange,oldvisstate,suicide;

public func AimAngle()     {return();}
public func ReadyToFire()  {return();}
public func IsAiming()     {return();}


/* Erstellung */

public func Set(object pClonk)
{
  //Anderer Todesschrei zur Unterscheidung zwischen Fake Death und "echtem" Ableben
  Sound("ClonkDie*.ogg");

  clonk = pClonk;
  SetPosition(GetX(pClonk),GetY(pClonk)/*GetObjHeight(pClonk)/2*/);
  SetXDir(GetXDir(pClonk));
  SetYDir(GetYDir(pClonk));

  //Clonk aufnehmen
  pClonk->Enter(this());
  //Objekte des Clonks aufnehmen
  GrabContents(pClonk);
  //Aussehen des Clonks imitieren
  SetGraphics (0,this(),GetID(pClonk),1,GFXOV_MODE_Object,0,0,pClonk);

  //Sichtwerte speichern
  oldvisrange = GetObjPlrViewRange(pClonk);
  oldvisstate = GetPlrFogOfWar(GetOwner(pClonk));

  //Sichtwerte für den Fake Death setzen
  SetPlrViewRange(100,pClonk);
  SetFoW(true,GetOwner(pClonk)); 

  //Lebenszeit setzen
  suicide = FKDT_SuicideTime;

  //Verzögert Auswahlmenü öffnen
  ScheduleCall(this(),"DoMenu",35,suicide); 
}

/* Auswahlmenü */

func DoMenu()
{
  suicide--;
  DeathMenu();
}

func DeathMenu()
{
  CloseMenu(clonk);
  CreateMenu(FKDT, clonk, this(), 0, GetName(), 0, C4MN_Style_Dialog, true);
  if(suicide > 0)
   AddMenuItem(Format("$SuicideW$",RGB(128,128,128),suicide), "NoSuicide", SKUL,clonk, 0, 0, "$DescSuicide$");
  else
   AddMenuItem("$Suicide$", "Suicide", SKUL,clonk, 0, 0, "$DescSuicide$");

  //SetMenuDecoration(MCDC, pClonk);
  SetMenuTextProgress(1, clonk); 
}

public func NoSuicide()
{
  Sound("ERROR",true,0,0,GetOwner(clonk)+1);
}

public func MenuQueryCancel() { return(true); }

/* Selbstmord */

public func Suicide()
{
  //Ende im Gelände
  clonk->Kill();

  //Leiche "auswerfen" und ausfaden lassen
  clonk->Exit(0,0,GetObjHeight(clonk)/2);
  clonk->FadeOut();

  //Verschwinden
  RemoveObject();
}

public func GetClonk() {return(clonk);}

/* Entfernen (bei Wiederbelebung) */

public func Destruction()
{
  //Kein Clonk?
  if(!clonk) return();

  //Clonk "auswerfen"
  if(Contained(clonk) == this())
   clonk->Exit(0,0,GetObjHeight(clonk)/2);

  //Besitztümer weitergeben
  if(GetAlive(clonk))
   clonk->GrabContents(this());
  else
   while(Contents())
    Exit(Contents(),0,+10);

  //Sichtdaten zurücksetzen
  SetFoW(oldvisstate,GetOwner(clonk));
  SetPlrViewRange(oldvisrange,clonk);
}

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