--���ﺰ���ű�

-- �������ս������
function c1003129_OnEnterCombat(MapID, InstanceID, CreatureID)
         
       cre.MonsterSay(MapID, InstanceID, CreatureID, 23)
       	
end

aux.RegisterCreatureEvent(1003129, 2, "c1003129_OnEnterCombat")

--������������
function c1003129_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 24)
       end	

end

aux.RegisterCreatureEvent(1003129, 4, "c1003129_OnDie")
