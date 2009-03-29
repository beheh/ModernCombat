#strict

global func InitCombineAI(object pObj)
{
  if(!pObj) pObj = this();

  var cbai = FindObject(CBAI);
  if(!cbai)
    cbai = CreateObject(CBAI,0,0,-1);

  cbai->Init(pObj);  
  
  return(cbai);
}


local iAIPlr;

protected func Initialize()
{
  iAIPlr = NO_OWNER;
  CreateScriptPlayer(GetName(),RGB(128,128,128),GameCall("CombineTeam"),CSPF_FixedAttributes|CSPF_NoScenarioInit|CSPF_Invisible|CSPF_NoEliminationCheck,GetID()); 
}

public func SetAIPlr(int iPlr)
{
  iAIPlr = iPlr;
}

public func GetAIPlr()
{
  return(iAIPlr);
}

protected func InitializeScriptPlayer(int iPlr, int iTeam)
{
  var cbai = FindObject(CBAI);
  if(!cbai)
  {
    Log("O_o Kein Combine-KI-Objekt. Aber ein ScriptSpieler dafür.");
    return();
  }
  
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetPlayerByIndex(i) != iPlr)
      SetHostility(GetPlayerByIndex(i),iPlr,true);
    
  cbai->SetAIPlr(iPlr);
  
  var i = GetEffectCount(0,this());
  var iEffect;
  while(i--)
    if(iEffect = GetEffect("ObjInit", this(), i))
    {
      EffectCall(this(),iEffect,"Timer");
      RemoveEffect(0, this(), iEffect);
    }
}

public func Init(object pObj)
{
  if(iAIPlr == NO_OWNER)
  {
    AddEffect("ObjInit",this(),10,10,this(),0,pObj);
    return();
  }

  if((GetOwner(pObj) != NO_OWNER) && (GetOwner(pObj) != iAIPlr))
    return();

  pObj->SetOwner(iAIPlr);
  MakeCrewMember(pObj,iAIPlr);
  SetupBot4K(Random(4)+1,pObj,10);
  SetAction("Jump");//O_o
}

public func FxObjInitStart(object pTarget, int iEffectNumber, int iTemp, pObj)
{
  EffectVar(0,pTarget,iEffectNumber) = pObj;
}

public func FxObjInitTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(pTarget->GetAIPlr() == NO_OWNER)
  {
    Log("O_o Combine-Script-Spieler ist (noch) nicht gejoint.");
    return(-1);
  }

  pTarget->Init(EffectVar(0,pTarget,iEffectNumber));
  return(-1);
}