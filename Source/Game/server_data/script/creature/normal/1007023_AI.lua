--���ﺰ���ű�

-- �������ս������
function c1007023_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40090)
       end	
end

aux.RegisterCreatureEvent(1007023, 2, "c1007023_OnEnterCombat")


--������������
function c1007023_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 40091)
       end	

end

aux.RegisterCreatureEvent(1007023, 4, "c1007023_OnDie")

