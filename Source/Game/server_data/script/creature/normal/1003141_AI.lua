--���ﺰ���ű�

-- �������ս������
function c1003141_OnEnterCombat(MapID, InstanceID, CreatureID)
       
             cre.MonsterSay(MapID, InstanceID, CreatureID, 45)
	
end

aux.RegisterCreatureEvent(1003141, 2, "c1003141_OnEnterCombat")


--������������
function c1003141_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 46)
       end	

end

aux.RegisterCreatureEvent(1003141, 4, "c1003141_OnDie")

