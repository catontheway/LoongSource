--���ﺰ���ű�

-- �������ս������
function c1003096_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 25)
       end	
end

aux.RegisterCreatureEvent(1003096, 2, "c1003096_OnEnterCombat")


--������������
function c1003096_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 26)
       end	

end

aux.RegisterCreatureEvent(1003096, 4, "c1003096_OnDie")

