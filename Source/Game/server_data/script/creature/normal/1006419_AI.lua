--���ﺰ���ű�

-- �������ս������
function c1006419_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 30056)
       end	
end

aux.RegisterCreatureEvent(1006419, 2, "c1006419_OnEnterCombat")


--������������
function c1006419_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 30057)
       end	

end

aux.RegisterCreatureEvent(1006419, 4, "c1006419_OnDie")

