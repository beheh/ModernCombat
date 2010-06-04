/*-- Effektsystem --*/

#strict 2

static const EFSM_Blood = 1;	//1/0
static const EFSM_BulletCasing = 2;	//1/0
static const EFSM_BulletEffects = 3;	//0/1/2
static const EFSM_Fog = 4;	//0/1
static const EFSM_ExplosionEffects = 5;	//0/1/2
static const EFSM_Enlight = 6;	//0/1

static EFSM_CurrentData;
static EFSM_Init;

global func EFSM_SetEffects(int iLevel) {
	if(!iLevel) iLevel = 3;
	EFSM_Init = true;
	EFSM_CurrentData = [];
	SetEffectData(iLevel>1, EFSM_Blood);
	SetEffectData(iLevel>2, EFSM_BulletCasing);
	SetEffectData(iLevel-1, EFSM_BulletEffects);
	SetEffectData(iLevel>1, EFSM_Fog);
	SetEffectData(iLevel-1, EFSM_ExplosionEffects);
	SetEffectData(iLevel>1, EFSM_Enlight);
	return true;
}

global func GetEffectData(int iEffect) {
	if(!EFSM_Init) EFSM_SetEffects();
	return EFSM_CurrentData[iEffect];
}

global func SetEffectData(int iData, int iEffect) {
	if(!EFSM_Init) EFSM_SetEffects();
	EFSM_CurrentData[iEffect] = iData;
	return true;
}
