/*-- Stylands --*/

#strict 2
#include CSTD


/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  //Abgewandelter Regelsatz: Kein FriendlyFire, Keine Munition, Keine Schwerverletzten, Flagge abnehmbar
  var array = [NOFF, NOAM, NOFD, FGRV];
  return array;
}

/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Einrichtung plazieren
  CreateInterior();
  //Ausr�stung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return(1);
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Das Spark-Objekt initialisieren
  RemoveAll(_SPK);
  CreateObject(_SPK, LandscapeWidth()/2 ,-5, -1)->SetAction("Wait");

  //Erdmaterialien
  //Ausr�stung
  EarthMaterials(FAPK,3,-1);
  EarthMaterials(DGNN,2,-1);
  EarthMaterials(BBTP,2,-1);
  EarthMaterials(C4PA,2,-1);
  EarthMaterials(RSHL,2,-1);
  EarthMaterials(CDBT,2,-1);

  //Granaten
  EarthMaterials(FGRN,3,-1);
  EarthMaterials(FRAG,4,-1);
  EarthMaterials(SGRN,4,-1);
  EarthMaterials(STUN,4,-1);
  EarthMaterials(PGRN,2,-1);
  EarthMaterials(EGRN,3,-1);
  EarthMaterials(VGRN,3,-1);

  //Waffen
  EarthMaterials(ASTR,1,-1);
  EarthMaterials(PPGN,1,-1);
  EarthMaterials(MNGN,1,-1);
  EarthMaterials(RTLR,1,-1);
  EarthMaterials(SGST,1,-1);
  EarthMaterials(SMGN,1,-1);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");
}

/* Bei Ausr�stung */

public func OnClassSelection(object pClonk)
{
  //Alte Flagge entfernen
  for(var flag in FindObjects(Find_ID(FLAG),Find_Owner(GetOwner(pClonk))))
  {
    CreateParticle("Blast",GetX(flag),GetY(flag),0,0,10*10);
    RemoveObject(flag);
  }

  //Fallschirm erstellen
  CreateObject(PARA,0,0,GetOwner(pClonk))->Set(pClonk);

  //Ausr�stung: Bausatz, Flagge, Erdgranate, Erdbebengranate, Jetpack
  CreateContents(CNKT,pClonk);
  CreateContents(FLAG,pClonk);
  CreateContents(EGRN,pClonk)->Activate(pClonk);
  CreateContents(VGRN,pClonk)->Activate(pClonk);
  CreateContents(JTPK, pClonk)->Activate(pClonk);
}

/* Regelw�hler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Battle Mode.mid;CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Battle Mode.mid",1);

  //Laufzeitbeitritt sperren
  SetMaxPlayer();
}

/* Bodenmaterialien plazieren */

global func EarthMaterials(def,amount,mat,x,y,wdt,hgt,fsave)
{
  var i,rndx,rndy,obj,j;
  while(i<amount)
  {
    if(wdt) rndx=x+Random(wdt);
      else rndx=x+Random(LandscapeWidth()-x);
    if(hgt) rndy=y+Random(hgt);
      else rndy=y+Random(LandscapeHeight()-y);

    if((mat!=-1 && GetMaterial(rndx,rndy)==mat) || (mat==-1 && GBackSolid(rndx,rndy)))
    {
      obj=CreateObject(def,rndx,rndy+GetDefCoreVal("Height","DefCore",def)/2, NO_OWNER);
      SetR(Random(360),obj);
      if(obj->~IsWeapon())
      {
        var ammo = obj->GetFMData(FM_AmmoID);
        var load = obj->GetFMData(FM_AmmoLoad);
        DoAmmo(ammo,-load, obj);
        DoAmmo(ammo, load, obj);
      }
      if(Stuck(obj)) i++;
        else RemoveObject(obj);
    } 

    if(!fsave && j++>5000) return(0);
  }
  return(j);
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  iX = Random(LandscapeWidth());
  iY = -20;
}