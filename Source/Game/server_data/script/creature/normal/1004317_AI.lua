--���ﺰ���ű�

-- �������ս������
function c1004317_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10039)
       end	
end

aux.RegisterCreatureEvent(1004317, 2, "c1004317_OnEnterCombat")


--������������
function c1004317_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 10040)
       end	

end

aux.RegisterCreatureEvent(1004317, 4, "c1004317_OnDie")

