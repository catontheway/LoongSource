--���ﺰ���ű�



--������������
function c1004011_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 10003)
       end	

end

aux.RegisterCreatureEvent(1004011, 4, "c1004011_OnDie")

