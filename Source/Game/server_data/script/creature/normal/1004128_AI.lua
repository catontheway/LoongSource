--���ﺰ���ű�

-- �������ս������
function c1004128_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10014)
       end	
end

aux.RegisterCreatureEvent(1004128, 2, "c1004128_OnEnterCombat")



--������������
function c1004128_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 10015)
       end	

end

aux.RegisterCreatureEvent(1004128, 4, "c1004128_OnDie")

