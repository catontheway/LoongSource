--���ﺰ���ű�

-- �������ս������
function c1005313_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20043)
       end	
end

aux.RegisterCreatureEvent(1005313, 2, "c1005313_OnEnterCombat")




--������������
function c1005313_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 20044)
       end	

end

aux.RegisterCreatureEvent(1005313, 4, "c1005313_OnDie")

