--���ﺰ���ű�

-- �������ս������
function c1004310_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10035)
       end	
end

aux.RegisterCreatureEvent(1004310, 2, "c1004310_OnEnterCombat")


--������������
function c1004310_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 10036)
       end	

end

aux.RegisterCreatureEvent(1004310, 4, "c1004310_OnDie")

