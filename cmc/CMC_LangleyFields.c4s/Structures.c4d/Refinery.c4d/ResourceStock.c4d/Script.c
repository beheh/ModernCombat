/*-- Ressourcenlager --*/

#strict 2

local basement, stock;	//Fundament

public func MaximumAmount() { return 500; } //Maximalanzahl in Credits


/* Initalisierung */

protected func Initialize()
{
  basement = CreateObject(BT02,0,8,GetOwner());
  AddEffect("ResourceStock", this, 100, 10, this);  
  Fill(MaximumAmount());

  return true;
}

public func Fill(int iAmount)
{
	stock = BoundBy(iAmount, 0, MaximumAmount());
	return true;
}

public func DoResources(int iChange)
{
	Fill(stock+iChange);
	return true;
}

public func GetResources() { return stock; }

/* Ressourcen ausgeben */

public func FxResourceStockStart(object pTarget, int iNr, int iTemp)
{
	if(iTemp)
		return;

	EffectVar(0, pTarget, iNr) = [];
	return true;
}

public func CarryLorry(object pTarget)
{
  if(GetProcedure(pTarget) != "PUSH") return;
  if(!GetActionTarget(0, pTarget)->~IsLorry()) return;
  
  var temp;
  if(GetActionTarget(0, pTarget)->~RejectCollect(RSCE, (temp = CreateContents(RSCE, this))))
  {
    if(temp)
      RemoveObject(temp);
    return;
  }
  
  if(temp)
    RemoveObject(temp);
  
  return true;
}

public func FxResourceStockTimer(object pTarget, int iNr)
{
  //Leer!
  if(!GetResources())
  	return;

	var clonkarray = EffectVar(0, pTarget, iNr);

  //Clonks beladen
  for(var clonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Distance(80)))
  {
    if(!GetResources()) return;
    //if(GetProcedure(clonk) == "PUSH" && GetActionTarget(0, clonk)->~IsLorry()) continue;
    if(GetEffect("ResourceStockCooldown", clonk)) continue;
    if(GetIndexOf(clonk, clonkarray) == -1)
    {
    	AddEffect("ResourceStockCooldown", clonk, 1, 5, this, 0, pTarget, iNr);
    	continue;
    }
    
    var obj = FindContents(RSCE, clonk), temp;
    if(!CarryLorry(clonk) && !clonk->~RejectCollect(RSCE, (temp = CreateContents(RSCE, this))))
    	if(obj && GetValue(obj) >= 15)
    		continue;

    var e;
    if(e = GetEffect("ResourceStockCheck", clonk))
    	EffectCall(clonk, e, "Clear");

    AddEffect("ResourceStockCheck", clonk, 1, 15, this, 0, pTarget, iNr);
    
    if(CarryLorry(clonk))
    {
    	var lorry = GetActionTarget(0, clonk), obj = 0;
      for(var o in FindObjects(Find_Container(lorry), Find_ID(RSCE)))
        if(GetValue(o) < 15)
          obj = o;
      
      if(!obj && !lorry->~RejectCollect(RSCE, temp))
      {
        obj = CreateContents(RSCE, lorry);
        obj->Set(Min(GetResources(), 5));
        DoResources(-Min(GetResources(), 5));

        var cnt = ObjectCount2(Find_Container(lorry), Find_ID(RSCE));
        Message("$LorryStock$", clonk, (cnt-1)*15+GetValue(obj));
      }
      //Wert des Warenobjektes um 1 erh�en
		  else if(GetValue(obj) < 15)
		  {
		  	obj->Set(GetValue(obj)+1);
		  	DoResources(-1);

		    var cnt = ObjectCount2(Find_Container(lorry), Find_ID(RSCE));
        Message("$LorryStock$", clonk, (cnt-1)*15+GetValue(obj));
		  }
		  else
		  	Message("$LorryStockFull$", clonk);
    }
    else
    {
		  if(!obj)
		  {
		  	obj = CreateContents(RSCE, clonk);
		    obj->Set(Min(GetResources(), 5));
		    DoResources(-Min(GetResources(), 5));
		    Message("$ClonkStock$", clonk, GetValue(obj), 15);
		  }
		  //Wert des Warenobjektes um 1 erh�en
		  else if(GetValue(obj) < 15)
		  {
		  	obj->Set(GetValue(obj)+1);
		  	DoResources(-1);
		    Message("$ClonkStock$", clonk, GetValue(obj), 15);
		  }
		  else
		  	Message("$ClonkStockFull$", clonk);
		}
		
		if(temp)
		  RemoveObject(temp);
  }
  
  //Raffinerie bef�llen
  var refinery = FindObject2(Find_ID(CRFY), Find_Distance(200), Sort_Distance());
  if(refinery)
  {
  	refinery->~Process(1);
  	DoResources(-1);
  }
  
  return true;
}

/* Hilfseffekte */

public func FxResourceStockCooldownStart(object pTarget, int iNr, int iTemp, object pStock, int iEffect)
{
	EffectVar(0, pTarget, iNr) = pStock;
	EffectVar(1, pTarget, iNr) = iEffect;
	
	return true;
}

public func FxResourceStockCooldownTimer(object pTarget, int iNr, int iTime)
{
	var objects = pTarget->FindObjects(pTarget->Find_AtPoint(), Find_ID(RSTK)), found = false;
	for(var obj in objects)
	{
	  if(obj == EffectVar(0, pTarget, iNr))
	  {
	    found = true;
	    break;
	  }
  }

	if(!found)
	  return -1;
	
	if(iTime > 35*4)
	{
		EffectVar(2, pTarget, iNr) = true;
		return -1;
	}
	
	return true;
}

public func FxResourceStockCooldownStop(object pTarget, int iNr)
{
	var temp = CreateContents(RSCE, this);
	if(EffectVar(2, pTarget, iNr) && !pTarget->~RejectCollect(RSCE, temp))
	{
		var stock = EffectVar(0, pTarget, iNr), e = EffectVar(1, pTarget, iNr);
		EffectVar(0, stock, e)[GetLength(EffectVar(0, stock, e))] = pTarget;
	}
	
	if(temp)
	  RemoveObject(temp);
	
	return true;
}

public func FxResourceStockCheckStart(object pTarget, int iNr, int iTemp, object pStock, int iEffect)
{
	EffectVar(0, pTarget, iNr) = pStock;
	EffectVar(1, pTarget, iNr) = iEffect;
	
	return true;
}

public func FxResourceStockCheckClear(object pTarget, int iNr)
{
  EffectVar(2, pTarget, iNr) = true;
  RemoveEffect("ResourceStockCheck", pTarget);

  return true;
}

public func FxResourceStockCheckStop(object pTarget, int iNr)
{
  //Clonk aus Liste l�schen
  if(!EffectVar(2, pTarget, iNr))
	{
		var stock = EffectVar(0, pTarget, iNr), e = EffectVar(1, pTarget, iNr);
		var array = EffectVar(0, stock, e), narray = [];
		
		for(var obj in array)
		  if(obj != pTarget)
		    narray[GetLength(narray)] = obj;
		
		EffectVar(0, stock, e) = narray;
	}
	
	return true;
}