--���ﺰ���ű�

-- �������ս������
function c1007111_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40016)
       end	
end

aux.RegisterCreatureEvent(1007111, 2, "c1007111_OnEnterCombat")

