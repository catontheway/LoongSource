--���ﺰ���ű�

-- �������ս������
function c1004235_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10031)
       end	
end

aux.RegisterCreatureEvent(1004235, 2, "c1004235_OnEnterCombat")


--������������
function c1004235_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 10032)
       end	

end

aux.RegisterCreatureEvent(1004235, 4, "c1004235_OnDie")

