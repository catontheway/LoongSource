--���ﺰ���ű�



--������������
function c1004008_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 10002)
       end	

end

aux.RegisterCreatureEvent(1004008, 4, "c1004008_OnDie")

