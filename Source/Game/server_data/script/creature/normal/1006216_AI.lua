--���ﺰ���ű�

-- �������ս������
function c1006216_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 30018)
       end	
end

aux.RegisterCreatureEvent(1006216, 2, "c1006216_OnEnterCombat")


--������������
function c1006216_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 30019)
       end	

end

aux.RegisterCreatureEvent(1006216, 4, "c1006216_OnDie")
