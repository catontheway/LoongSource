--���ﺰ���ű�

-- �������ս������
function c1005310_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20036)
       end	
end

aux.RegisterCreatureEvent(1005310, 2, "c1005310_OnEnterCombat")



--������������
function c1005310_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 20037)
       end	

end

aux.RegisterCreatureEvent(1005310, 4, "c1005310_OnDie")

