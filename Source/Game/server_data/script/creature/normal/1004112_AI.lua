--���ﺰ���ű�

-- �������ս������
function c1004112_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10006)
       end	
end

aux.RegisterCreatureEvent(1004112, 2, "c1004112_OnEnterCombat")


--������������
function c1004112_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 10007)
       end	

end

aux.RegisterCreatureEvent(1004112, 4, "c1004112_OnDie")

