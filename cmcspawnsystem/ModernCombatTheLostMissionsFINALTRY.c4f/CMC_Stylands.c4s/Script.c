/*-- Stylands --*/

#strict
#include CSTD


/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  //Abgewandelter Regelsatz: Kein FriendlyFire, Fallschaden, Keine Schwerverletzten, Flagge abnehmbar
  var array = [NOFF, FDMG, NOFD, FGRV];
  return(array);
}

/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Einrichtung plazieren
  CreateInterior();
  return(1);
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Erdmaterialien
  //Ausrüstung
  EarthMaterials(DGNN,3,-1);
  EarthMaterials(FAPK,2,-1);
  EarthMaterials(BTBG,2,-1);
  EarthMaterials(RSHL,2,-1);
  EarthMaterials(CDBT,1,-1);
  EarthMaterials(C4PA,1,-1);

  //Granaten
  EarthMaterials(SGRN,4,-1);
  EarthMaterials(STUN,4,-1);
  EarthMaterials(FGRN,3,-1);
  EarthMaterials(FRAG,3,-1);
  EarthMaterials(EGRN,3,-1);
  EarthMaterials(VGRN,3,-1);
  EarthMaterials(PGRN,2,-1);

  //Waffen
  EarthMaterials(ATWN,2,-1);
  EarthMaterials(ASTR,2,-1);
  EarthMaterials(PPGN,1,-1);
  EarthMaterials(MNGN,1,-1);
  EarthMaterials(RTLR,1,-1);
  EarthMaterials(SGST,1,-1);
  EarthMaterials(SMGN,1,-1);

  //Munition
  if(!FindObject(NOAM))
  {
   EarthMaterials(ABOX,4,-1);
   EarthMaterials(GBOX,3,-1);
   EarthMaterials(MBOX,2,-1);
  }
}

/* Bei Clonkausrüstung */

public func OnClonkEquip(object pClonk)
{
  //Nicht bei Regelwahl
  if(!g_chooserFinished) 
   return;

  //Alte Flagge entfernen
  for(var flag in FindObjects(Find_ID(FLAG),Find_Owner(GetOwner(pClonk))))
  {
   CreateParticle("Blast",GetX(flag),GetY(flag),0,0,10*10);
   RemoveObject(flag);
  }

  //Fallschirm erstellen
  AddEffect("IntPara", pClonk, 1, 1);

  //Ausrüstung: Revolver, Bausatz, Flagge, Erdgranate, Erdbebengranate, Fallschirmrucksack
  var wpn = CreateContents(RVLR, pClonk);
  wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
  CreateContents(CNKT,pClonk);
  CreateContents(FLAG,pClonk);
  CreateContents(EGRN,pClonk)->Activate(pClonk);
  CreateContents(VGRN,pClonk)->Activate(pClonk);
  CreateContents(PPAR,pClonk)->Activate(pClonk);

  //Munition: 90 Kugeln, 12 Granaten, 3 Raketen
  if(!FindObject(NOAM))
  {
   DoAmmo(MIAM, 3, pClonk);
   DoAmmo(GRAM, 12, pClonk);
   DoAmmo(STAM, 90, pClonk);
  }
}

global func FxIntParaTimer(object pTarget)
{
  CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
  return -1;
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Battle Mode.mid;CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Battle Mode.mid",1);

  //Spark-Objekt initialisieren
  RemoveAll(_SPK);
  CreateObject(_SPK, LandscapeWidth()/2 ,-5, -1)->SetAction("Wait");

  //Keine Munition-Regel nicht vorhanden: Munition kaufbar
  if(!FindObject(NOAM))
  {
   for(var i = 0; i < GetPlayerCount(); i++)
   {
     DoHomebaseMaterial(GetPlayerByIndex(i) + 1, ABOX, 10);
     DoHomebaseMaterial(GetPlayerByIndex(i) + 1, GBOX, 8);
     DoHomebaseMaterial(GetPlayerByIndex(i) + 1, MBOX, 5);

     DoHomebaseProduction(GetPlayerByIndex(i) + 1, ABOX, 5);
     DoHomebaseProduction(GetPlayerByIndex(i) + 1, GBOX, 4);
     DoHomebaseProduction(GetPlayerByIndex(i) + 1, MBOX, 2);
   }
  }
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