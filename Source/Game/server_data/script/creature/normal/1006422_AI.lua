--���ﺰ���ű�


--������������
function c1006422_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 30061)
       end	

end

aux.RegisterCreatureEvent(1006422, 4, "c1006422_OnDie")

