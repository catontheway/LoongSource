--���ﺰ���ű�

-- �������ս������
function c1003069_OnEnterCombat(MapID, InstanceID, CreatureID)
       
             cre.MonsterSay(MapID, InstanceID, CreatureID, 61)
      	
end

aux.RegisterCreatureEvent(1003069, 2, "c1003069_OnEnterCombat")


--������������
function c1003069_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 62)
       end	

end

aux.RegisterCreatureEvent(1003069, 4, "c1003069_OnDie")

