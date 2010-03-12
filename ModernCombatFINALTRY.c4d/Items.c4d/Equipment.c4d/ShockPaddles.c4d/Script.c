/*-- Defibrillator --*/

#strict

local charge;

public func HandSize() {return(1000);}
public func HandX() {return(4500);}
public func HandY() {return(0);}
public func IsDrawable() {return(true);}
public func MaxEnergy() {return(30);}

func IsEquipment(){return(true);}


/* Initalisierung */

public func Initialize()
{
  charge = MaxEnergy(); //Defibrillator geladen
}

public func GetUser()
{
  return(Contained());
}

/* Akkuregeneration */

public func Timer()
{
  //Hinweis bei voller Ladung
  if(GetUser())
   if(charge == 9)
    Sound("CDBT_Reload*.ogg");

  //Akku um einen Punkt aufladen
  charge = BoundBy(charge+1,0,MaxEnergy());

  return(true);
}

/* Shocken */

public func ControlThrow(pByObject)
{
  Use(pByObject);
  return(true);
}

public func Activate(pClonk)
{
  return(Use(pClonk));
}

public func ControlUp(object pClonk)
{
  if(GetPlrDownDouble(GetController(pClonk)))
  {
   pClonk->~StartHeal(MDIC_HealMode_Reanimate);
   return(true);
  }
  return(false);
}

func Use(caller)
{
  //Hinweis bei fehlender Spannung
  if(charge <= 9)
    PlayerMessage(GetOwner(caller), "$NotCharged$", Contained(this),ENAM);
  
  //Nicht in Gebäuden
  if(Contained(caller)) return(false);

  //Nicht schocken wenn nicht bereit
  if(!Ready()) return(false);

  //Richtung feststellen
  var dir = +1;
  if(GetDir(GetUser()) == DIR_Left())
    dir = -1;

  //Patienten suchen
  var obj;
  if(obj = FindObject2(Find_InRect(-10,-10,20,20),Find_ID(FKDT),Find_Allied(GetOwner(caller)),Find_NoContainer()))
  {
    obj = obj->GetClonk();

    //Patient wiederbeleben
    obj->StopFakeDeath();

    //Effekte
    Sound("CDBT_Shock.ogg");
    obj->Sound("ClonkCough*.ogg");
    obj->Sparks(10,RGB(250,150,0));
    obj->Sparks(5,RGB(100,100,250));
    obj->AddLightFlash(40+Random(20),0,0,RGB(0,140,255));

    //Globalnachricht
    EventInfo4K(0,Format("$Reanimated$",GetPlayerName(GetOwner(caller)),GetPlayerName(GetOwner(obj))),GetID(),GetPlrColorDw(GetOwner(caller)));

    //Energie entladen
    charge = BoundBy(charge-20,0,MaxEnergy());

    return(1);
  }

  obj=0;

  //Keine Patienten, dann eben Feinde suchen
  if(FindObject2(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Exclude(caller)))
  {
    obj = FindObjects(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Exclude(caller));
    for(var target in obj)
    {
      if(target && CheckEnemy(GetUser(),target))
      {
        //Schaden durch elektrischen Schlag (und Schleudern)
        DoDmg(40+Random(10),DMG_Energy,target);
        Fling(target,2*dir,-2);

        if(!target)//Könnte ja jetzt weg sein.
          target = this();

        //Effekte
        Sound("CDBT_Shock.ogg");
        target->Sparks(5,RGB(250,150,0));
        target->Sparks(10,RGB(100,100,250));
        target->AddLightFlash(40+Random(20),0,0,RGB(0,140,255));

        //Energie entladen
        charge = BoundBy(charge-20,0,MaxEnergy());
      }
    }
  }
  else
  {
    //Effekte
    Sound("CDBT_ShockFail.ogg");
    Sparks(5,RGB(250,150,0));
    Sparks(10,RGB(100,100,250));
    AddLightFlash(40+Random(20),0,0,RGB(0,140,255));

    //Energie entladen
    charge = BoundBy(charge-10,0,MaxEnergy());//Schock ins Leere kostet weniger Energiepunkte
  }

  //Nachladen
  SetAction("Reload");

  return(true);
}

/* Bereitschaft */

func Ready()
{
  //Nicht wenn am Nachladen
  if(GetAction() eq "Reload")
   return(false);

  //Nicht bei zu wenig Spannung
  if(charge >= 10)
   return(true);

  return(false);
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
  {
   if(ContentsCount(GetID(),pObj))
    return(true);
  }
  return(false);
}

/* HUD */

func CustomHUD(){return(true);}

func UpdateHUD(object pHUD)
{
  pHUD->Charge(charge,MaxEnergy());
  pHUD->Ammo(charge, MaxEnergy(), GetName(), true);
}

/* Sounds */

protected func Hit()
{
  Sound("WPN2_Hit*.ogg");
}

protected func Selection()
{
  Sound("CDBT_Charge.ogg");
}