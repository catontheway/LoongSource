--���ﺰ���ű�


--������������
function c1008310_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 40059)
       end	

end

aux.RegisterCreatureEvent(1008310, 4, "c1008310_OnDie")

