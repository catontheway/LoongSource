--���ﺰ���ű�

-- �������ս������
function c1003104_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 31)
       end	
end

aux.RegisterCreatureEvent(1003104, 2, "c1003104_OnEnterCombat")

