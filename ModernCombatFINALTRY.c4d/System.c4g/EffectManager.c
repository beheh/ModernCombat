/*-- Effektsystem --*/

//Übergibt dem Effektmanager Standardverhalten bei Effektstufeneinstellungen.

#strict 2

static EFSM_CurrentData;
static EFSM_Init;
static EFSM_Level;
static EFSM_Dark;


/* Effect-Konstanten */

static const EFSM_Blood = 1;		//1/0
static const EFSM_BulletCasing = 2;	//1/0
static const EFSM_BulletEffects = 3;	//0/1/2
static const EFSM_Fog = 4;		//0/1
static const EFSM_ExplosionEffects = 5;	//0/1/2
static const EFSM_Enlight = 6;		//0/1
static const EFSM_Darkness = 7;		//0/1
static const EFSM_Waratmosphere = 8;	//0/1
static const EFSM_Deco = 9;		//0/1
static const EFSM_Lensflares = 10;	//0/1/2

/* Globale Effektstufe setzen */

global func EFSM_SetEffects(int iLevel)
{
  if(!iLevel) iLevel = 3;
  if(iLevel == EFSM_Level)
    return true;
  EFSM_Init = true;
  EFSM_CurrentData = [];
  EFSM_Level = iLevel;
  SetEffectData(iLevel>1, EFSM_Blood);
  SetEffectData(iLevel>2, EFSM_BulletCasing);
  SetEffectData(iLevel-1, EFSM_BulletEffects);
  SetEffectData(iLevel>1, EFSM_Fog);
  SetEffectData(iLevel-1, EFSM_ExplosionEffects);
  SetEffectData(iLevel>1, EFSM_Enlight);
  SetEffectData(iLevel>2, EFSM_Darkness);
  SetEffectData(iLevel>1, EFSM_Waratmosphere);
  SetEffectData(iLevel>1, EFSM_Deco);
  SetEffectData(iLevel-1, EFSM_Lensflares);
  return true;
}

global func GetEffectData(int iEffect)
{
  if(!EFSM_Init) EFSM_SetEffects();
  return EFSM_CurrentData[iEffect];
}

global func SetEffectData(int iData, int iEffect)
{
  if(!EFSM_Init) EFSM_SetEffects();
  EFSM_CurrentData[iEffect] = iData;
  OnUpdateEffects(iEffect);
  return true;
}

global func OnUpdateEffects(int iEffect)
{
  //Dunkelheit aktualisieren
  if(iEffect == EFSM_Darkness)
  { 
    if(EFSM_Level == 3)
    { 
      if(!IsDark())
      {
        CreateObject(DARK); 
        Schedule(Format("SetDarkness(%d)", EFSM_Dark), 2);

        //Alle Lichtobjekte sichtbar schalten
        for(var light in FindObjects(Find_Func("IsLamp")))
        {
          if(light && light->LocalN("bOn"))
            light->TurnOn();
        }
      }
    }
    else
    {
      if(IsDark())
        EFSM_Dark = GetDarkness();
      RemoveAll(DARK);

      //Alle Lichtobjekte unsichtbar schalten
      for(var light in FindObjects(Find_Func("IsLight"),Find_Not(Find_Func("IsModernLight"))))
        SetVisibility(VIS_None,light);
    }
  }

  //Gewitterwolken entfernen/hinzufügen
  if(EFSM_Level == 3 && FindObject(STRM))
  {
    ClearParticles("Cloud");
    DoClouds();
  }
  else
  {
    ClearParticles("Cloud");
  }

  //Spezifische Szenariodekorationen entfernen/einfügen
  if(iEffect == EFSM_Deco && !GetEffectData(EFSM_Deco))
  {
    for(var back in FindObjects(Find_Func("IsDeco")))
      RemoveObject(back);
  }
  if(iEffect == EFSM_Deco && GetEffectData(EFSM_Deco) && !FindObject2(Find_Func("IsDeco")))
    GameCall("CreateOptionalFeatures");
}